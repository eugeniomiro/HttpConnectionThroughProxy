#ifndef HTTP_CONNECTOR_H
#define HTTP_CONNECTOR_H

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/url/parse.hpp>
#include <string>
#include <optional>

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
using tcp = asio::ip::tcp;
using namespace boost::urls;

class HttpConnector
{
public:
    HttpConnector(asio::io_context& ioc, std::optional<std::string> proxyUrl = std::nullopt) : _ioc(ioc), _socket(ioc) 
    {
        _proxyUrl = parse_uri(proxyUrl.value_or(""));
    }

    // Connect and return a plain TCP Beast stream
    beast::tcp_stream connectPlain(const std::string& host, const std::string& port)
    {
        if (_proxyUrl)
        {
            connectViaProxy(host, port);
        }
        else
        {
            tcp::resolver resolver(_ioc);
            auto endpoints = resolver.resolve(host, port);
            beast::tcp_stream stream(_ioc);
            stream.connect(endpoints);
            return stream;
        }
        return beast::tcp_stream(std::move(_socket));
    }

    // Connect and return an SSL Beast stream
    beast::ssl_stream<beast::tcp_stream> connectSSL(const std::string& host, const std::string& port, asio::ssl::context& ssl_ctx)
    {
        if (_proxyUrl)
        {
            connectViaProxy(host, port);
        }
        else
        {
            tcp::resolver resolver(_ioc);
            auto endpoints = resolver.resolve(host, port);
            beast::tcp_stream direct_stream(_ioc);
            direct_stream.connect(endpoints);
            return beast::ssl_stream<beast::tcp_stream>(std::move(direct_stream), ssl_ctx);
        }

        beast::tcp_stream beast_stream(std::move(_socket));
        beast::ssl_stream<beast::tcp_stream> ssl_stream(std::move(beast_stream), ssl_ctx);
        ssl_stream.handshake(asio::ssl::stream_base::client);
        return ssl_stream;
    }

private:
    asio::io_context&               _ioc;
    boost::system::result<url_view> _proxyUrl;
    tcp::socket                     _socket;

    void connectViaProxy(const std::string& host, const std::string& port)
    {
        tcp::resolver resolver(_ioc);

        auto proxy_endpoints = resolver.resolve(_proxyUrl->host(), _proxyUrl->port());
        asio::connect(_socket, proxy_endpoints);

        // Send CONNECT request
        std::string connect_req =
            "CONNECT " + host + ":" + port + " HTTP/1.1\r\n"
            "Host: " + host + ":" + port + "\r\n"
            "Proxy-Connection: Keep-Alive\r\n\r\n";

        asio::write(_socket, asio::buffer(connect_req));

        // Read proxy response
        beast::flat_buffer buffer;
        http::response_parser<http::empty_body> parser;
        parser.skip(true);
        http::read(_socket, buffer, parser);

        if (parser.get().result() != http::status::ok)
        {
            auto reason = parser.get().reason();
            throw std::runtime_error("Proxy tunnel failed: " + std::string(reason.data(), reason.size()));
        }
    }
};

#endif // !HTTP_CONNECTOR_H
