#include "BaseConfig.h"
#include <coroutine>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <thread>
#include <tuple>
#include <im/base/coroutine.h>
#include <set>

#ifndef ROC_COROTINE_H
#define ROC_COROTINE_H

using namespace roc::coro;
// -------------------------------------------------------
template<typename type>
inline void request1(CoroPromise<type> promise, type value, int ms) {

    auto func = [promise, value, ms]() mutable{
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        promise.set_value(value);
    };

    static int ss = 0;

    ++ss;
    if (ss%2) {
        net_io_context.post(func);
    } else {
        main_io_context.post(func);
    }

    //net_io_context.post(func);
    //std::thread(func).detach();
}

// 子协程，返回一个Task<int>
template<typename type>
inline co_async<type> child_coroutine(type value, int ms) {
    type res = co_await co_awaitable_wapper<type>{[value, ms](CoroPromise<type> promise) {
        request1(promise, value, ms);
    }};
    co_return res;
}

inline co_async<> child_coroutine_void() {
    co_return;
}

// 父协程，调用子协程
inline co_async<int> parent_coroutine(int input) {
    int res = co_await child_coroutine(input, 1000);
    co_return res * 2;
}

inline co_async<> co_main() {
    int res = co_await parent_coroutine(100); 
    co_return;
}

inline void test();

int all = 1000;
int cur = 0;

std::mutex mutt;

inline co_async<> await_tasks_test() {
    std::vector<int> vec{1,2,3,4,5};
    auto a1 = child_coroutine(100, 0);
    auto a2 = child_coroutine(89.80, 0);
    auto a3 = child_coroutine("string", 0);
    auto a4 = child_coroutine_void();
    auto res = co_await when_any(std::make_tuple(a1,a2,a3,a4));

    auto res2 = co_await when_all(std::make_tuple(
        child_coroutine_void(),
        child_coroutine(100, 89),
        child_coroutine(200, 10),
        child_coroutine("string", 13),
        child_coroutine(std::set<int>{1,2,3,4,5,6}, 10)
    ));
    auto res_copy = res;
 
    int cc = 0;
    {
        std::lock_guard<std::mutex> lock(mutt);
        ++cur;
        cc = cur;
    }
    if (cc >= all) {
        std::cout<<"success"<<std::endl;
        {
            std::lock_guard<std::mutex> lock(mutt);
            cur = 0;
        }
        test();
    }
}


inline void test() {
    for (int i = 0; i < all; ++i) {
        await_tasks_test();
    }
    co_main();
}

#endif