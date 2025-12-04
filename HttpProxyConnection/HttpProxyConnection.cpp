// HttpProxyConnection.cpp : Defines the entry point for the application.
//

#include "HttpProxyConnection.h"

int main(int argc, char* argv[])
{
    try 
    {
        std::string proxy_url;

        if (argc > 1)
        {
            proxy_url = argv[1];
        }
        std::cout << "Using proxy: '" << proxy_url << "'\n";

        asio::io_context ioc;
        asio::ssl::context ssl_ctx(asio::ssl::context::tls_client);
        ssl_ctx.set_default_verify_paths();
        ssl_ctx.set_verify_mode(asio::ssl::verify_peer);

        HttpConnector connector(ioc, proxy_url);

        auto stream = connector.connectPlain("www.example.com", "80");

        namespace http = boost::beast::http;

        http::request<http::string_body> req{ http::verb::get, "/", 11 };
        req.set(http::field::host, "www.example.com");
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        http::write(stream, req);

        boost::beast::flat_buffer buffer;
        http::response<http::string_body> res;
        http::read(stream, buffer, res);

        std::cout << res << "\n";

        boost::system::error_code ec;
        if (ec == asio::error::eof) ec = {};
        if (ec) throw boost::system::system_error(ec);

    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
