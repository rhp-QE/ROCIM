#ifndef NET_CLIENT_H
#define NET_CLIENT_H

#include <boost/asio.hpp>
#include <functional>
#include <linux/stat.h>
#include <vector>

namespace roc::net {

struct LongConnectionConfig {
    std::string host;
    std::string port;
    uint32_t reconnect_interval = 5000; // 重连间隔，单位毫秒
};

using ReceiveCallback = std::function<void(const std::vector<char>&)>;
using ConnectCallback = std::function<void()>;

template<typename T>
class ILongConnection {
public:
    virtual ~ILongConnection() = default;
    void set_receive_callback(ReceiveCallback callback);
    void set_connect_callback(ConnectCallback callback);
    void send(const std::vector<char>& data);
    void connect();
    void disconnect();
};

template<typename T>
void ILongConnection<T>::set_receive_callback(ReceiveCallback callback) {
    static_cast<T*>(this)->set_receive_callback_impl(callback);
}

template<typename T>
void ILongConnection<T>::set_connect_callback(ConnectCallback callback) {
    static_cast<T*>(this)->set_connect_callback_impl(callback);
}

template<typename T>
void ILongConnection<T>::send(const std::vector<char>& data) {
    static_cast<T*>(this)->send_impl(data);
}

template<typename T>
void ILongConnection<T>::connect() {
    static_cast<T*>(this)->connect_impl();
}

template<typename T>
void ILongConnection<T>::disconnect() {
    static_cast<T*>(this)->disconnect_impl();
}

} // namespace roc::net

#endif // NET_CLIENT_H