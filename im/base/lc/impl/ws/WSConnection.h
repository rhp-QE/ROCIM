//
// WCConnection.h
//
// author: Ruan Huipeng
// date : 2025-03-23
// 

#ifndef ROC_NET_WCCONNECTION_H
#define ROC_NET_WCCONNECTION_H

#include "im/base/lc/Interface/ICoroLongConnection.h"
#include <memory>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/asio/use_awaitable.hpp>


namespace roc::base::net {


class WSConnection : public ICoroLongConnection<WSConnection>, public std::enable_shared_from_this<WSConnection> {

    using tcp = boost::asio::ip::tcp;
    using websocket = boost::beast::websocket::stream<tcp::socket>;

friend ICoroLongConnection<WSConnection>;

private:
    boost::asio::awaitable<bool> connect_impl(LCConfig config);

    boost::asio::awaitable<bool> disconnect_impl();

    boost::asio::awaitable<size_t> send_impl(void *data, size_t size); 

    boost::asio::awaitable<boost::beast::flat_buffer> read_impl();

    LCConfig config_;

    bool connected_;

    std::unique_ptr<websocket> ws;
};

} // namespace roc::net

#endif // ROC_NET_WCCONNECTION_H
