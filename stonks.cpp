#include <stdio.h>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <iostream>
#include <thread>
#include <vector>
// #include <boost/asio/ssl/stream_base.hpp>

std::string hostname = "www.alphavantage.co";

namespace io = boost::asio;
namespace ip = io::ip;
using tcp = ip::tcp;
using error_code = boost::system::error_code;

// Here we go
namespace ssl = io::ssl;
using ssl_socket = ssl::stream<tcp::socket>;

std::vector<char> vBuffer(1024);

void GetData(ssl_socket &sock)
{
    sock.async_read_some(io::buffer(vBuffer.data(), vBuffer.size()),
                         [&](error_code ecc, std::size_t length)
                         {
                             if (!ecc)
                             {
                                 std::cout << "Get Data: " << length << " bytes\n";

                                 for (int i = 0; i < length; i++)
                                 {
                                     std::cout << vBuffer[i];
                                 }

                                 GetData(sock);
                             }
                             else
                             {
                                 std::cout << "Error: " << ecc.message() << std::endl;
                             }
                         });
}

int main(int argc, char *argv[])
{
    error_code ec;

    io::io_context io_context;
    io::io_context::work idleWork(io_context);

    std::thread thrContext = std::thread([&]()
                                         { io_context.run(); });

    ssl::context ssl_context(ssl::context::tls);
    ssl_context.set_default_verify_paths();
    ssl_socket socket(io_context, ssl_context);

    tcp::resolver resolver(io_context);
    // tcp::resolver::query query(hostname, "https");
    auto endpoints = resolver.resolve(hostname, "443");
    io::connect(socket.next_layer(), endpoints, ec);

    // io::connect(socket.lowest_layer(), resolver.resolve(query), ec);
    socket.lowest_layer().set_option(tcp::no_delay(true));

    if (!ec)
    {
        std::cout << "Connected!" << std::endl;
    }
    else
    {
        std::cout << "Failed to connect to address: " << ec.message() << std::endl;
    }

    socket.set_verify_mode(ssl::verify_peer);
    socket.set_verify_callback(ssl::host_name_verification(hostname));

    // Set SNI Hostname (many hosts need this to handshake successfully)
    if (!SSL_set_tlsext_host_name(socket.native_handle(), hostname.c_str()))
    {
        throw boost::system::system_error(
            ::ERR_get_error(), boost::asio::error::get_ssl_category());
    }

    socket.handshake(ssl_socket::client, ec);

    if (!ec)
    {
        std::cout << "Handshake done!" << std::endl;
    }
    else
    {
        std::cout << "Failed to handshake: " << ec.message() << std::endl;
    }

    GetData(socket);

    std::string request =
        "GET /query?function=TIME_SERIES_MONTHLY&symbol=IBM&apikey=02PX8JZ63RHDIAUN HTTP/1.1\r\n"
        "Host: www.alphavantage.co\r\n"
        "Connection: close\r\n\r\n";

    // io::write(socket, io::buffer(request));
    socket.write_some(io::buffer(request.data(), request.size()), ec);
    std::cout << "Write done" << std::endl;

    using namespace std::literals::chrono_literals;
    std::this_thread::sleep_for(2000ms);

    // io::streambuf response;
    // io::read(socket, response, ec);

    // if (ec)
    // {
    //     std::cout << ec.message() << std::endl;
    // }

    // std::cout << "Read done" << std::endl;
    // std::cout << std::istream(&response).rdbuf() << "\n";

    io_context.stop();
    if (thrContext.joinable())
        thrContext.join();

    return 0;
}