#include <boost/asio.hpp>
#include <functional>
#include <queue>
#include <iostream>
#include <memory>

namespace roc::net {

struct LongConnectionConfig {
    std::string host;
    std::string port;
    uint32_t reconnect_interval = 5000; // 重连间隔，单位毫秒
};

class Client : public std::enable_shared_from_this<Client> {
public:
    using ReceiveCallback = std::function<void(const std::vector<char>&)>;
    using ConnectCallback = std::function<void()>;
    
    static std::shared_ptr<Client> create(
        boost::asio::io_context& io_context,
        const LongConnectionConfig& config
    ) {
        return std::shared_ptr<Client>(new Client(io_context, config));
    }

    ~Client() {
        disconnect();
    }

    void set_receive_callback(ReceiveCallback callback) {
        receive_callback_ = std::move(callback);
    }

    void set_connect_callback(ConnectCallback callback) {
        connect_callback_ = std::move(callback);
    }

    void send(const std::vector<char>& data) {
        boost::asio::post(io_context_, [this, data]() {
            bool write_in_progress = !write_queue_.empty();
            write_queue_.push(data);
            if (!write_in_progress) {
                do_write();
            }
        });
    }

    void connect() {
        if (!connected_) {
            do_connect();
        }
    }

    void disconnect() {
        connected_ = false;
        boost::system::error_code ec;
        socket_.close(ec);
        reconnect_timer_.cancel();
    }

private:
    Client(boost::asio::io_context& io_context, const LongConnectionConfig& config)
        : io_context_(io_context),
          socket_(io_context),
          resolver_(io_context),
          reconnect_timer_(io_context),
          config_(config) {}

    void do_connect() {
        resolver_.async_resolve(
            config_.host,
            config_.port,
            [self = shared_from_this()](const auto& ec, const auto& endpoints) {
                if (ec) {
                    std::cout<<"resolve addr error"<<std::endl;
                    self->schedule_reconnect();
                    return;
                }

                boost::asio::async_connect(
                    self->socket_,
                    endpoints,
                    [self](const auto& ec, const auto&) {
                        if (!ec) {
                            std::cout<<"connect success"<<std::endl;
                            self->connected_ = true;
                            self->connect_callback_();
                            self->do_read();
                        } else {
                            std::cout<<"connect error"<<std::endl;
                            self->schedule_reconnect();
                        }
                    });
            });
    }

    void do_read() {
        auto self = shared_from_this();
        socket_.async_read_some(
            boost::asio::buffer(read_buffer_),
            [self](const auto& ec, size_t bytes_transferred) {
                if (!ec) {
                    if (self->receive_callback_) {
                        std::vector<char> data(
                            self->read_buffer_.data(),
                            self->read_buffer_.data() + bytes_transferred
                        );
                        self->receive_callback_(data);
                    }
                    self->do_read();
                } else {
                    self->handle_disconnect(ec);
                }
            });
    }

    void do_write() {
        auto self = shared_from_this();
        const auto& data = write_queue_.front();
        std::cout<<"send data: "<<std::endl;
        boost::asio::async_write(
            socket_,
            boost::asio::buffer(data),
            [self](const auto& ec, size_t) {
                if (ec) {
                    self->handle_disconnect(ec);
                    return;
                }

                self->write_queue_.pop();
                if (!self->write_queue_.empty()) {
                    self->do_write();
                }
            });
    }

    void schedule_reconnect() {
        connected_ = false;
        reconnect_timer_.expires_after(
            std::chrono::milliseconds(config_.reconnect_interval));
        
        auto self = shared_from_this();
        reconnect_timer_.async_wait(
            [self](const auto& ec) {
                if (!ec) {
                    self->do_connect();
                }
            });
    }

    void handle_disconnect(const boost::system::error_code& ec) {
        if (connected_) {
            connected_ = false;
            boost::system::error_code ignore_ec;
            socket_.close(ignore_ec);
            schedule_reconnect();
        }
    }

    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::ip::tcp::resolver resolver_;
    boost::asio::steady_timer reconnect_timer_;
    LongConnectionConfig config_;
    ReceiveCallback receive_callback_;
    ConnectCallback connect_callback_;
    std::array<char, 1024> read_buffer_;
    std::queue<std::vector<char>> write_queue_;
    bool connected_ = false;
};

} // namespace roc::net