//
// ICoroLongConnection.h
//
// author: Ruan Huipeng
// date : 2025-03-23
// 

#ifndef ROC_NET_ICOROLONGCONNECTION_H
#define ROC_NET_ICOROLONGCONNECTION_H

#include <boost/asio/awaitable.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <cstddef>
namespace roc::base::net {

struct LCConfig {
    std::string host;
    std::string port;
    uint32_t reconnect_interval = 5000; // 重连间隔，单位毫秒
};

template<typename T>
class ICoroLongConnection {
public:

    boost::asio::awaitable<bool> connect(LCConfig config);
    boost::asio::awaitable<bool> disconnect();
    boost::asio::awaitable<size_t> send(void *data, size_t size); 
    boost::asio::awaitable<boost::beast::flat_buffer> read();

};

// -------------- impl --------------

template <typename T>
boost::asio::awaitable<bool> ICoroLongConnection<T>::connect(LCConfig config) {
    return static_cast<T*>(this) -> connect_impl(config);
}

template <typename T>
boost::asio::awaitable<bool> ICoroLongConnection<T>::disconnect() {
    return static_cast<T*>(this) -> disconnect_impl();
}

template <typename T>
boost::asio::awaitable<size_t> ICoroLongConnection<T>::send(void *data, size_t size) {
    return static_cast<T*>(this) -> send_impl(data, size);
}

template <typename T>
boost::asio::awaitable<boost::beast::flat_buffer> ICoroLongConnection<T>::read() {
    return static_cast<T*>(this) -> read_impl();
}

} // namespace roc::bae::net

#endif // ROC_NET_ICOROLONGCONNECTION_H
