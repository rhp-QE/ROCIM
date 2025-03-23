#include "BaseConfig.h"
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/use_awaitable.hpp>



boost::asio::awaitable<int> cor1() {
    boost::asio::steady_timer timer(main_io_context, 1);
    co_await timer.async_wait(boost::asio::use_awaitable);
    co_return 100; 
}


boost::asio::awaitable<int> cor2() { 
    int res = co_await cor1();
    co_return res;
}

void boostCoroTest() {
    boost::asio::co_spawn(net_io_context, cor2(), boost::asio::detached);
    return;
}