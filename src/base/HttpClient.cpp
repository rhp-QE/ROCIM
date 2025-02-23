#include <base/HttpClient.h>

#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <boost/beast/http/field.hpp>
#include <iostream>
#include <memory>
#include <thread>
#include <boost/json.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace json = boost::json;
using tcp = boost::beast::net::ip::tcp;


namespace roc::net {

class Session : public std::enable_shared_from_this<Session> {
    tcp::resolver resolver;
    tcp::socket socket;
    beast::flat_buffer buffer;
    http::request<http::empty_body> req;
    http::response<http::dynamic_body> res;
    std::function<void(const std::string str)> callback;
    boost::asio::steady_timer timer;
    roc::net::Reuqest request;

public:
    Session(roc::net::Reuqest request, boost::asio::io_context& ioc) :
        socket(ioc),
        timer(ioc),
        resolver(ioc),
        request(request) {}

    void start() {

        auto self = shared_from_this(); // 关键：保持对象存活
        self->callback = callback;

        req.method(http::verb::get);
        req.target(request.url_);
        req.set(http::field::host, request.host_);
        req.set(http::field::user_agent, "Boost.Beast");

        // 设置超时（可选）
        timer.expires_after(std::chrono::seconds(5));
        timer.async_wait([self](beast::error_code ec) {
            if (!ec) {
                self->socket.cancel();
            }
        });

        // 异步解析域名（直接使用 IP 时可跳过，但需保持逻辑一致）
        resolver.async_resolve(
            request.host_, 
            request.port_,
            [self](const boost::system::error_code& ec, tcp::resolver::results_type results) {
                if (ec) return self->fail(ec, "resolve");
                self->async_connect(results);
            }
        );

    }

    void async_connect(tcp::resolver::results_type results) {
        boost::beast::net::async_connect(
            socket, 
            results,
            [self = shared_from_this()](beast::error_code ec, tcp::endpoint) {
                if (ec) return self->fail(ec, "connect");
                self->async_write_request();
            }
        );
    }

    void async_write_request() {
        http::async_write(socket, req, [self = shared_from_this()](beast::error_code ec, size_t) {
            if (ec) return self->fail(ec, "write");
            self->async_read_response();
        });
    }

    void async_read_response() {
        http::async_read(socket, buffer, res, [self = shared_from_this()](beast::error_code ec, size_t) {
            if (ec) return self->fail(ec, "read");

            std::string str = beast::buffers_to_string(self->res.body().data());

            if (self->request.callback_) {
                self->request.callback_(str);
            }
        });
    }

    void fail(beast::error_code ec, const char* what) {
        std::cerr << "[错误] " << what << ": " << ec.message() << "\n";
    }
};


void roc::net::Reuqest::request() {
    static boost::asio::io_context io_context;
    static auto wotk = boost::beast::net::make_work_guard(io_context);
    static std::thread thread([]{
        io_context.run();
    });

    auto session = std::make_shared<Session>(*this, io_context);

    session->start();
}

}// namespace::roc::net