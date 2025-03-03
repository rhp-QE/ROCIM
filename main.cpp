#include <boost/asio/io_context.hpp>
#include <test/RequestTest.h>
#include <test/buffertest.h>
#include <thread>

#include "BaseConfig.h"

boost::asio::io_context net_io_context;
boost::asio::io_context main_io_context;

int main() {

    boost::asio::io_context::work work(net_io_context);
    std::thread net_thread([]{
        net_io_context.run();
    });

    roc::im::sdk::net::testProtobuf();
    // testLongConAndBuffer();

    boost::asio::io_context::work work_main(main_io_context);
    main_io_context.run();
    
    return 0;
}