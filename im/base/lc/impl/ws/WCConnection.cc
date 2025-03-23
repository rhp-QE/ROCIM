// WCConnection.cc
//
// author: Ruan Huipeng
// date : 2025-03-23
//

#include "WCConnection.h"
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <memory>
#include <string>

namespace asio = boost::asio;
namespace beast = boost::beast;
using tcp = boost::asio::ip::tcp;
using websocket = beast::websocket::stream<tcp::socket>;

// 使用命名空间
namespace roc::net {

boost::asio::awaitable<bool> WCConnection::connect_impl(LCConfig config) {
    bool conencted = false;

    try {

        auto executor = co_await asio::this_coro::executor;
        tcp::resolver resolver(executor);
        ws = std::make_unique<websocket>(executor);

        // 解析目标主机和端口
        auto const results = co_await resolver.async_resolve(config.host, config.port, asio::use_awaitable);

        // 连接 TCP
        co_await asio::async_connect(ws->next_layer(), results, asio::use_awaitable);

        // WebSocket 握手
        co_await ws->async_handshake(config.host, "/", asio::use_awaitable);

    } catch (const std::exception& e) {

        conencted = false;

    }

    co_return conencted;
}

boost::asio::awaitable<bool> WCConnection::disconnect_impl() {
    co_return true;
}

boost::asio::awaitable<size_t> WCConnection::send_impl(void *data, size_t size) {
    co_return ws->async_write(asio::buffer(data, size), asio::use_awaitable);
}

boost::asio::awaitable<boost::beast::flat_buffer> WCConnection::read_impl() {
    beast::flat_buffer rx_buf;
    co_await ws->async_read(rx_buf, asio::use_awaitable);
    co_return rx_buf;
}

} // namespace roc::net
