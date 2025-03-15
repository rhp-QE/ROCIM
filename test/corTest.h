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
inline void request1(CoroPromise<> promise) {
    std::cout<<"request 1"<<std::endl;

    auto func = [promise]() mutable{
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        std::cout<<"response 1"<<std::endl;
        promise.set_value();
    };

    std::thread(std::move(func)).detach();
}

// 子协程，返回一个Task<int>
inline co_async<int> child_coroutine(int value) {
    std::cout << "Child coroutine started with value: " << value << "\n";
    co_await co_awaitable_wapper<>{[](CoroPromise<> promise) {
        request1(std::move(promise));
    }};
    co_return 100 * 2;
}

// 父协程，调用子协程
inline co_async<int> parent_coroutine(int input) {
    std::cout << "Parent coroutine started\n";
    int child_result = co_await child_coroutine(input);;
    printf("child %d", child_result);
    co_return child_result * 2;
}

inline co_async<> co_main() {
    int res = co_await parent_coroutine(100); 
    printf("parent %d", res);
    co_return;
}


inline void test() {
    co_main();
}

#endif