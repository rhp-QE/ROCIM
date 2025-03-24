#include <boost/asio/io_context.hpp>
#include <test/corTest.h>
#include <test/buffertest.h>
#include <thread>

#include "BaseConfig.h"
#include <im/base/coroutine.h>
#include <test/httpTest.h>
#include <tuple>
#include <utility>
#include <test/coCostTime.h>
#include <test/boostCoroTest.h>
#include <test/testwc.h>

boost::asio::io_context net_io_context;
boost::asio::io_context main_io_context;

namespace roc::coro {
std::unordered_map<size_t, std::shared_ptr<CoroRAII>> coro_manager;
std::mutex coro_mutex;
}

template <typename Ty>
void print(Ty aa) {
    Ty v = aa;
    std::cout<<v<<std::endl;
}

template<size_t... I>
void print_index(std::index_sequence<I...>) {
    (..., print(I));
}

template <typename... Retype>
void test_tuple(std::tuple<Retype...> res) {
    auto index = std::make_index_sequence<std::tuple_size<decltype(res)>::value>{};
    [&]<size_t... I>(std::index_sequence<I...>) {
        (..., (std::cout<<std::get<I>(res)<<std::endl));
    }(index);
}

int main() {

    // std::tuple<int, double, std::string> ss(1, 1.1111, "fdasfa");
    // std::apply([](auto&&... args) {
    //     auto index = std::make_index_sequence<sizeof...(args)>();
    //     (..., (print(args)));
    // }, ss);


    // print_index(std::index_sequence<1,2,3,4,5,6>());

   // test_tuple(std::make_tuple(1,2,3,4,5,6));

    //roc::im::sdk::net::testProtobuf();
    //testLongConAndBuffer();
    //testHttp();
    //test();

    co_cost_time_test();

    // boostCoroTest();
    //testWC();

    boost::asio::io_context::work work(net_io_context);
    std::thread net_thread([]{
        net_io_context.run();
    });

    boost::asio::io_context::work work_main(main_io_context);
    main_io_context.run();
    
    return 0;
}