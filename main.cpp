#include <boost/asio/io_context.hpp>
#include <iostream>
#include <memory>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>


#include <net/ILongConnection.h>
#include <net/LongConnectionImpl.h>

#include <base/LinkBuffer.h>

#include "buffertest.h"
#include "httpTest.h"

#include <sdk_module/test/sdk_test/ReuqestTest.h>

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(boost::asio::io_context& io_context)
        : socket_(io_context) {}

    tcp::socket& socket() {
        return socket_;
    }

    void start() {
        // 异步读取数据
        socket_.async_read_some(
            boost::asio::buffer(data_, max_length),
            boost::bind(
                &Session::handle_read, shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred
            )
        );
    }

private:
    void handle_read(const boost::system::error_code& error, std::size_t bytes_transferred) {
        if (!error) {
            // 打印接收到的数据
            std::cout << "Received: " << std::string(data_, bytes_transferred) << std::endl;

            // 继续读取数据
            start();
        } else {
            std::cerr << "Error: " << error.message() << std::endl;
            // 发生错误时，Session 对象会被自动销毁（因为使用了 shared_ptr）
        }
    }

    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
};

class Server {
public:
    Server(boost::asio::io_context& io_context, short port)
        : io_context_(io_context),
          acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        start_accept();
    }

private:
    void start_accept() {
        // 创建新的 Session 对象
        auto new_session = std::make_shared<Session>(io_context_);

        // 异步接受连接
        acceptor_.async_accept(
            new_session->socket(),
            boost::bind(
                &Server::handle_accept, this, new_session,
                boost::asio::placeholders::error
            )
        );
    }

    void handle_accept(std::shared_ptr<Session> new_session, const boost::system::error_code& error) {
        if (!error) {
            // 启动 Session
            new_session->start();
        } else {
            std::cerr << "Accept error: " << error.message() << std::endl;
        }

        // 继续接受新的连接
        start_accept();
    }

    boost::asio::io_context& io_context_;
    tcp::acceptor acceptor_;
};


void test1() {
    try {
        boost::asio::io_context io_context;
        Server server(io_context, 8888);  // 监听端口 7777
        io_context.run();  // 运行事件循环
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}


void testBuffer() {
    using namespace::roc::base;
    using namespace::std;

    std::shared_ptr<NetBuffer> buffer = std::make_shared<NetBuffer>();

    string str = "rhpmark";

    buffer->write(str.data(), str.length());
    std::string res = buffer->get_read_buffers().value().readString();

    str = "appendstr";
    buffer->write(str.data(), str.length());
    std::string res1 = buffer->get_read_buffers().value().readString();


    cout<<res<<endl;
    cout<<":"<<res1<<":"<<endl;

    while(true) {

    }
}

int main() {
   // testBuffer();
    // testLongConAndBuffer();
    // testUniquePtr();
    // test();
    // testHttp();
    roc::im::sdk::net::test();

    while(true) {}
    return 0;
}