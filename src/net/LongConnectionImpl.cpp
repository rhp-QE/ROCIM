#include "base/LinkBuffer.h"
#include <iostream>
#include <memory>
#include <net/ILongConnection.h>
#include <net/LongConnectionImpl.h>

namespace roc::net {

LongConnectionImpl::LongConnectionImpl(boost::asio::io_context& io_context, const LongConnectionConfig& config)
    : io_context_(io_context),
      socket_(io_context),
      resolver_(io_context),
      reconnect_timer_(io_context),
      config_(config),
      write_buffer_(std::make_shared<base::NetBuffer>()),
      read_buffer_(std::make_shared<base::NetBuffer>()){}

void LongConnectionImpl::send_impl(const std::string& data) { 
       if (data.length() <= 0) {
            return;
        }
        write_buffer_->write(data.data(), data.length());
        do_write();
}

void LongConnectionImpl::send_impl(const char* data, size_t size) { 
    if (size < 0) {
         return;
     }
     std::cout<<"[call send_impl]"<<std::endl;
     write_buffer_->write(data, size);
     do_write();
}

void LongConnectionImpl::connect_impl() {
    if (!connected_) {
        do_connect();
    }
}

void LongConnectionImpl::set_connect_callback_impl(ConnectCallback callback) {
    connect_callback_ = std::move(callback);
}

void LongConnectionImpl::set_receive_callback_impl(ReceiveCallback callback) {
    receive_callback_ = std::move(callback);
}

void LongConnectionImpl::disconnect_impl() {
    connected_ = false;
    boost::system::error_code ec;
    socket_.close(ec);
    reconnect_timer_.cancel();
}

void LongConnectionImpl::do_connect() {
    auto self = shared_from_this();
    resolver_.async_resolve(
        config_.host,
        config_.port,
        [self](const boost::system::error_code& ec, const auto& endpoints) {
            if (ec) {
                std::cout << "[resolve addr error]" << std::endl;
                self->schedule_reconnect();
                return;
            }

            boost::asio::async_connect(
                self->socket_,
                endpoints,
                [self](const boost::system::error_code& ec, const auto&) {
                    if (!ec) {
                        std::cout << "[connect success]" << std::endl;
                        self->connected_ = true;
                        if (self->connect_callback_) {
                            self->connect_callback_();
                        }
                        self->do_read();
                    } else {
                        std::cout << "[connect error]" << std::endl;
                        self->schedule_reconnect();
                    }
                });
        });
}

void LongConnectionImpl::do_read() {
    auto self = shared_from_this();
    socket_.async_read_some(
        self->read_buffer_->prepare_buffers(),
        [self](const boost::system::error_code& ec, size_t bytes_transferred) {
            if (!ec) {
                self->read_buffer_->commit(bytes_transferred);
                if (self->receive_callback_) {
                    self->receive_callback_(self->read_buffer_.get());
                }
                self->do_read();
            } else {
                self->handle_disconnect(ec);
            }
        });
}

void LongConnectionImpl::do_write() {
    auto self = shared_from_this();
    
    auto readResult = self->write_buffer_->get_read_buffers();

    auto str = readResult.readString();

    boost::asio::async_write(
        socket_,
        readResult.buffers,
        [self, readResult](const boost::system::error_code& ec, size_t) {
            if (ec) {
                self->handle_disconnect(ec);
                return;
            }

           // auto str = self->write_buffer_->get_read_buffers().readString();

          //  if (str.length()) {
          //      self->do_write();
          //  }
        });
}

void LongConnectionImpl::schedule_reconnect() {
    connected_ = false;
    reconnect_timer_.expires_after(
        std::chrono::milliseconds(config_.reconnect_interval));
    
    auto self = shared_from_this();
    reconnect_timer_.async_wait(
        [self](const boost::system::error_code& ec) {
            if (!ec) {
                self->do_connect();
            }
        });
}

void LongConnectionImpl::handle_disconnect(const boost::system::error_code& ec) {
    if (connected_) {
        connected_ = false;
        boost::system::error_code ignore_ec;
        socket_.close(ignore_ec);
        schedule_reconnect();
    }
}

} // namespace roc::net