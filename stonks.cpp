#include "config.h"
#include "common.h"

namespace io = boost::asio;
namespace ip = io::ip;
namespace ssl = io::ssl;

using tcp = ip::tcp;
using error_code = boost::system::error_code;
using ssl_socket = ssl::stream<tcp::socket>;

using namespace Xcurse;
using namespace std::literals::chrono_literals;

int main(int argc, char *argv[])
{
    // error code
    error_code ec;
    // asio context
    io::io_context io_context;
    // idle work
    io::io_context::work idleWork(io_context);
    // context thread
    std::thread thrContext = std::thread([&]()
                                         { io_context.run(); });
    // resolver
    tcp::resolver resolver(io_context);
    // ssl context
    ssl::context ssl_context(ssl::context::tls);
    ssl_context.set_default_verify_paths();

    // Xcurse
    Display::init();
    Display &d = *Display::get_display();
    d.set_refresh_interval(200);

    bool add_main_win = d.add_obj("root", "trend", new TrendChartWindow("trend", 1));

    TrendChartWindow *trend_win = static_cast<TrendChartWindow *>(d["trend"]);

    trend_win->set_title(L" Main ");
    d.map_key_action('x', [&]() -> void
                     { d.power_off(); });
    d.power_on();
    // make new socket for every new connection
    while (d.has_power())
    {
        // ssl socket
        ssl_socket socket(io_context, ssl_context);
        // HTTPS endpoint
        auto endpoints = resolver.resolve(DOMAIN, "443");
        /// connect endpoint
        io::connect(socket.next_layer(), endpoints, ec);
        socket.lowest_layer().set_option(tcp::no_delay(true));

        if (ec)
            std::cout << "Failed to connect to address: " << ec.message() << std::endl;

        // setup SSL
        socket.set_verify_mode(ssl::verify_peer);
        socket.set_verify_callback(ssl::host_name_verification(DOMAIN));

        // Set SNI Hostname (many hosts need this to handshake successfully)
        if (!SSL_set_tlsext_host_name(socket.native_handle(), DOMAIN.c_str()))
            throw boost::system::system_error(
                ::ERR_get_error(), boost::asio::error::get_ssl_category());

        // SSL handshake
        socket.handshake(ssl_socket::client, ec);

        if (ec)
            std::cout << "Failed to handshake: " << ec.message() << std::endl;

        API_Request r("GET");
        r.add_param("function", "TIME_SERIES_INTRADAY");
        r.add_param("symbol", "TSLA");
        r.add_param("interval", "1min");
        r.add_param("apikey", API_KEY);
        // API request
        std::string request = r.to_string();

        // write request to buffer
        socket.write_some(io::buffer(request.data(), request.size()), ec);

        if (ec)
            std::cout << "Error writing request to buffer: " << ec.message() << std::endl;

        // read response from socket
        io::streambuf response;
        io::read(socket, response, ec);

        if (ec)
            std::cout << "Error reading response from socket: " << ec.message() << std::endl;

        // storing response to string
        std::ostringstream ss;
        ss << std::istream(&response).rdbuf();
        std::string s = ss.str();
        // std::cout << s << std::endl;

        DataSet data_set;
        parse_data(data_set, s);

        // trend_win->set_data();
        std::vector<float> d_open, d_close;
        regex_save<float>(d_open, s, e_open, [](const std::string &s) -> float
                          { return std::stof(s); });
        regex_save<float>(d_close, s, e_close, [](const std::string &s) -> float
                          { return std::stof(s); });
        trend_win->set_data(d_open, d_close);
        std::this_thread::sleep_for(20s);
        // data_adaptor(data_set);
    }

    // joining context thread
    io_context.stop();
    if (thrContext.joinable())
        thrContext.join();

    d.power_off();

    return 0;
}