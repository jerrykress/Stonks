#include "constant.h"
#include "api.h"
#include "data.h"
#include "parser.h"

namespace io = boost::asio;
namespace ip = io::ip;
namespace ssl = io::ssl;

using tcp = ip::tcp;
using error_code = boost::system::error_code;
using ssl_socket = ssl::stream<tcp::socket>;

using namespace Xcurse;
using namespace std::literals::chrono_literals;

static bool prog_exit = false; // exit signal

int main(int argc, char *argv[])
{
    /*
        Build API Request
    */
    API_Request r(argc, argv);

    /*
        Setup Asio
    */
    error_code ec;                             // error code
    io::io_context io_context;                 // asio context
    io::io_context::work idleWork(io_context); // idle work
    std::thread thrContext = std::thread([&]()
                                         { io_context.run(); }); // context thread
    tcp::resolver resolver(io_context);                          // resolver
    ssl::context ssl_context(ssl::context::tls);                 // ssl context
    ssl_context.set_default_verify_paths();

    /*
        Setup Xcurse windows
    */
    Display::init();
    Display &d = *Display::get_display();
    d.set_refresh_interval(200);

    d.add_obj("root", "title", new TextField("Stock Name", ALIGN_CENTER));
    d.add_obj("root", "price", new TrendChartWindow(1));
    d.add_obj("root", "vol", new BarChartWindow(1));
    d.add_obj("root", "helpbar", new TextField("[-]Zoom out  [+]Zoom in  [P]Price   [V]Volume   [X]Quit", ALIGN_CENTER));

    auto price_win = static_cast<TrendChartWindow *>(d["price"]);
    auto vol_win = static_cast<BarChartWindow *>(d["vol"]);
    auto title_widget = static_cast<TextField *>(d["title"]);
    auto help_widget = static_cast<TextField *>(d["helpbar"]);

    price_win->set_title(L" Price ");
    vol_win->set_title(L" Volume ");

    title_widget->background = BACKGROUND_COLOR_BLUE;
    title_widget->foreground = TEXT_COLOR_BRIGHT_BLACK;
    title_widget->bold = true;

    help_widget->background = BACKGROUND_COLOR_BRIGHT_CYAN;
    help_widget->foreground = TEXT_COLOR_BRIGHT_BLACK;

    /*
        Key mapping
    */
    d.map_key_action('x', [&]() -> void
                     { prog_exit = true; });
    d.map_key_action('p', [&]() -> void
                     { price_win->set_visible(); });
    d.map_key_action('v', [&]() -> void
                     { vol_win->set_visible(); });
    d.map_key_action('-', [&]() -> void
                     { r.prev_func(); });
    d.map_key_action('=', [&]() -> void
                     { r.next_func(); });

    d.power_on();

    /*
        Xcurse runtime
    */
    while (!prog_exit)
    {
        // get new request string everytime to monitor changes
        std::string request = r.to_string();

        ssl_socket socket(io_context, ssl_context);
        auto endpoints = resolver.resolve(DOMAIN, "443");
        io::connect(socket.next_layer(), endpoints, ec);
        socket.lowest_layer().set_option(tcp::no_delay(true));

#ifdef DEBUG
        if (ec)
            std::cout << "Failed to connect to address: " << ec.message() << std::endl;
#endif
        // setup SSL
        socket.set_verify_mode(ssl::verify_peer);
        socket.set_verify_callback(ssl::host_name_verification(DOMAIN));

        // Set SNI Hostname (hosts need this to handshake successfully)
        if (!SSL_set_tlsext_host_name(socket.native_handle(), DOMAIN.c_str()))
            throw boost::system::system_error(
                ::ERR_get_error(), boost::asio::error::get_ssl_category());

        // SSL handshake
        socket.handshake(ssl_socket::client, ec);

#ifdef DEBUG
        if (ec)
            std::cout << "Failed to handshake: " << ec.message() << std::endl;
#endif
        // write request to buffer
        socket.write_some(io::buffer(request.data(), request.size()), ec);

#ifdef DEBUG
        if (ec)
            std::cout << "Error writing request to buffer: " << ec.message() << std::endl;
#endif
        // read response from socket
        io::streambuf response;
        io::read(socket, response, ec);

#ifdef DEBUG
        if (ec)
            std::cout << "Error reading response from socket: " << ec.message() << std::endl;
#endif
        // storing response to string
        std::ostringstream ss;
        ss << std::istream(&response).rdbuf();
        std::string s = ss.str();

        DataSet ds;
        parse_dataset(ds, s);

        title_widget->set_data(to_wstring(r.get_name() + " (" + r.get_func() + ", " + ds.dates.back().to_string() + (r.get_func() == "Intra-day" ? (" " + ds.times.back().to_string()) : "") + ") "));
        price_win->set_data(ds.low, ds.high, ds.close);
        price_win->set_title(to_wstring(" Price :" + std::to_string(ds.close.back()) + " "));
        vol_win->set_data(ds.volume);
        vol_win->set_title(to_wstring(" Volume :" + std::to_string(ds.volume.back()) + " "));

        std::this_thread::sleep_for(std::chrono::seconds(60 / requests_pm));
    }

    // joining context thread
    io_context.stop();
    if (thrContext.joinable())
        thrContext.join();

    d.power_off();

    return 0;
}