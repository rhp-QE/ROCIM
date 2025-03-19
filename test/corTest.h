#include "BaseConfig.h"
#include <coroutine>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <optional>
#include <thread>
#include <unordered_map>
#include <im/base/coroutine.h>

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

    net_io_context.post(func);
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

inline co_async<> await_all_test() {
    std::vector<int> vec{1,2,3,4,5};
    auto a2 = child_coroutine(89.80, 0);
    auto a1 = child_coroutine(100, 0);
    auto a3 = child_coroutine("string", 0);
    auto res = co_await when_any(std::make_tuple(a1,a2,a3));
    auto res_copy =   res;
}


inline void test() {
    await_all_test();
    co_main();
}

#endif