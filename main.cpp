#include <boost/asio/io_context.hpp>
#include <iostream>
#include <memory>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>


#include <net/ILongConnection.h>
#include <net/LongConnectionImpl.h>

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

void test2() {
    // boost::asio::io_context io_context;
    
    // roc::net::LongConnectionConfig config1 = {"127.0.0.1", "8181", 30000 };


    // auto client = roc::net::Client::create(io_context, config1);
    
    // client->set_receive_callback([](const std::vector<char>& data) {
    //     std::cout<<"receive data : "<<std::string(data.begin(), data.end())<<std::endl;
    // });

    // client->set_connect_callback([&client]() {
    //     std::cout<<"connect success up"<<std::endl;
    //     // 发送数据示例
    //     client->send({'H', 'e', 'l', 'l', 'o'});
    // });
    
    // client->connect();

    // // 在其他线程运行io_context
    // std::thread io_thread([&io_context](){
    //     io_context.run();
    // });
    
   
    // ...其他业务逻辑
    while(true){}
}

void test3() {
    using namespace::roc::net;
    using LongConnectionType = ILongConnection<LongConnectionImpl>;

    boost::asio::io_context io_context;
    
    roc::net::LongConnectionConfig config = {"127.0.0.1", "8181", 30000 };

    std::shared_ptr<LongConnectionType> conn = std::make_shared<LongConnectionImpl>(io_context, config);

    std::cout<<"ok1"<<std::endl;

    conn->set_receive_callback([](const std::vector<char>& data) {
        std::cout<<"receive data : "<<std::string(data.begin(), data.end())<<std::endl;
    });

    std::cout<<"ok1"<<std::endl;
    conn->set_connect_callback([&conn]() {
        std::cout<<"connect success up"<<std::endl;
        // 发送数据示例
        conn->send({'H', 'e', 'l', 'l', 'o'});
    });
    
    std::cout<<"ok1"<<std::endl;
    conn->connect();

    std::cout<<"ok1"<<std::endl;
    std::cout<<"ok1"<<std::endl;

    // 在其他线程运行io_context
    std::thread io_thread([&io_context](){
        io_context.run();
    });
    
   
    // ...其他业务逻辑
    std::string str;
    while(std::cin>>str){
        conn->send(std::vector<char>(str.begin(), str.end()));
    }
}

int main() {
    test3();
    return 0;
}