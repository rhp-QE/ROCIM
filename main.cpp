#include <boost/asio/detail/concurrency_hint.hpp>
#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_context.hpp>
#include <memory>
#include <test/buffertest.h>
#include <test/corTest.h>
#include <thread>

#include "BaseConfig.h"
#include <im/base/coroutine.h>
#include <test/boostCoroTest.h>
#include <test/channelTest.h>
#include <test/coCostTime.h>
#include <test/httpTest.h>
#include <test/testwc.h>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <expected>

boost::asio::io_context net_io_context{BOOST_ASIO_CONCURRENCY_HINT_UNSAFE_IO};
boost::asio::io_context main_io_context{BOOST_ASIO_CONCURRENCY_HINT_UNSAFE_IO};

namespace roc::coro {
std::unordered_map<size_t, std::shared_ptr<CoroRAII>> coro_manager;
std::mutex coro_mutex;
} // namespace roc::coro


std::expected<int, std::string> testex() {
    return std::unexpected("123");
}

template <typename Ty> void print(Ty aa) {
    Ty v = aa;
    std::cout << v << std::endl;
}

template <size_t... I> void print_index(std::index_sequence<I...>) {
    (..., print(I));
}

template <typename... Retype> void test_tuple(std::tuple<Retype...> res) {
    auto index =
        std::make_index_sequence<std::tuple_size<decltype(res)>::value>{};
    [&]<size_t... I>(std::index_sequence<I...>) {
        (..., (std::cout << std::get<I>(res) << std::endl));
    }(index);
}

struct Node {
    ~Node() { std::cout << "faf" << std::endl; }
};

int main() {

    // std::tuple<int, double, std::string> ss(1, 1.1111, "fdasfa");
    // std::apply([](auto&&... args) {
    //     auto index = std::make_index_sequence<sizeof...(args)>();
    //     (..., (print(args)));
    // }, ss);

    std::unordered_map<std::string, std::unique_ptr<int>> map;
    map["123"] = std::make_unique<int>(1);

    // print_index(std::index_sequence<1,2,3,4,5,6>());

    // test_tuple(std::make_tuple(1,2,3,4,5,6));

    // roc::im::sdk::net::testProtobuf();
    // testLongConAndBuffer();
    // testHttp();
    // test();

    {
        std::unique_ptr<Node> ptr = std::make_unique<Node>();
        auto pp = ptr.get();
    }


    auto res = testex();

    // co_cost_time_test();

    // boostCoroTest();
    // testWC();
    testChannel();

    auto wark_work = boost::asio::make_work_guard(net_io_context);
    std::thread net_thread([] { net_io_context.run(); });

    auto main_work = boost::asio::make_work_guard(main_io_context);
    main_io_context.run();

    return 0;
}
