#include <boost/asio/io_context.hpp>
#include <cstdint>
#include <iostream>
#include <memory>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>


#include <net/ILongConnection.h>
#include <net/LongConnectionImpl.h>

#include <base/LinkBuffer.h>
#include <random>
#include <variant>

/// 发送前
char buf[6096];

/// 接受数据
char bbb[6096];

size_t size;

size_t hash;

// 定义字符范围
const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

/// 生成数据
void product() {
        // 创建随机数生成器
        std::random_device rd;  // 获取随机数种子
        std::mt19937 gen(rd()); // 以随机数种子初始化生成器
        std::uniform_int_distribution<> distrib(0, 6000); // 定义范围
    
        // 生成随机数
        size = distrib(gen) + 10;

    hash = 0;
    for (int i = 0; i < size; ++i) {
        std::random_device rd; // 随机数种子
        std::mt19937 gen(rd()); // 初始化生成器
        std::uniform_int_distribution<> distrib(0, sizeof(charset) - 2); // 分布范围

        buf[i] = charset[distrib(gen)];

        hash = (hash * 131) + buf[i];
    }
}

/// 数据检验
void check() {
    size_t hh = 0;
    for (int i = 0; i < size; ++i) {
        hh = (hh * 131) + bbb[i];
    }

    std::cout<<"[hash]"<<hash <<" "<<hh<<std::endl;
    if (hh != hash) {
        std::cerr<<"[error]"<<std::endl;
    }
}

int cc = 10000;

void testLongConAndBuffer() {
    using namespace::roc::net;
    using LongConnectionType = ILongConnection<LongConnectionImpl>;

    boost::asio::io_context io_context;
    
    roc::net::LongConnectionConfig config = {"127.0.0.1", "8182", 30000 };

    std::shared_ptr<LongConnectionType> conn = std::make_shared<LongConnectionImpl>(io_context, config);

    int cnt = 1;

    conn->set_receive_callback([&cnt, conn](std::shared_ptr<LongConnectionType> con, roc::base::NetBuffer::ReadResult data) {
        
        auto sizee = data.readBytes(bbb, size);

        data.release();

        check();

        cc-=1;

        std::cout<<"[left receive count]"<<cc<<std::endl;
        if (cc > 0) {
            product();
            conn->send(buf, size);
        }
    });

    conn->set_connect_callback([](std::shared_ptr<LongConnectionType> con) {
        std::cout<<"[connect success up]"<<std::endl;
        // 发送数据示例
        product();
        con->send(buf, size);
    });
    
    conn->connect();


    // 在其他线程运行io_context
    std::thread io_thread([&io_context](){
        io_context.run();
    });


    // ...其他业务逻辑
    while(true){
    }
}


void test() {
    uint32_t number = 1234567;
    
    char number_bytes[4];

    for (size_t i = 0; i < sizeof(uint32_t); ++i) {
        number_bytes[i] = static_cast<char>(number & 0xFF);
        number >>= 8;  // 右移 8 位处理下一字节
    }


    uint32_t result = 0;
    for(int i = 0; i < 4; ++i) {
        result |= static_cast<uint8_t>(number_bytes[i]) << (i<<3);
    }

    std::cout<<"[number]"<<result<<std::endl;
}