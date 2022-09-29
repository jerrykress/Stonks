#include "config.h"
#include "common.h"

namespace io = boost::asio;
namespace ip = io::ip;
namespace ssl = io::ssl;

using tcp = ip::tcp;
using error_code = boost::system::error_code;
using ssl_socket = ssl::stream<tcp::socket>;
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

    // make new socket for every new connection
    for (int i = 0; i < 5; i++)
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
        r.add_param("symbol", "AAPL");
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
        std::cout << s << std::endl;

        DataSet data_set;
        parse_data(data_set, s);

        // std::cout << data_set << std::endl;

        std::this_thread::sleep_for(20s);
        data_adaptor(data_set);
    }

    // joining context thread
    io_context.stop();
    if (thrContext.joinable())
        thrContext.join();

    return 0;
}