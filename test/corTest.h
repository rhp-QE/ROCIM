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
inline void request1(CoroPromise<int> promise, int value) {

    auto func = [promise, value]() mutable{
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        promise.set_value(value);
    };

    main_io_context.post(func);
}

// 子协程，返回一个Task<int>
inline co_async<int> child_coroutine(int value) {
    int res = co_await co_awaitable_wapper<int>{[value](CoroPromise<int> promise) {
        request1(std::move(promise), value);
    }};
    co_return res * 2;
}

// 父协程，调用子协程
inline co_async<int> parent_coroutine(int input) {
    int res = co_await child_coroutine(input);;
    co_return res * 2;
}

inline co_async<> co_main() {
    int res = co_await parent_coroutine(100); 
    co_return;
}


inline void test() {
    co_main();
}

#endif