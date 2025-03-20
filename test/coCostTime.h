#include <iostream>
#include <chrono>
#include <coroutine>
#include <memory>
#include <im/base/coroutine.h>


using namespace roc::coro;


int func() {
    int a = 1;
    for (int i = 0; i < 100; ++i) {
        if (i%2) {
            a += i;
        } else {
            a -= i;
        }

        if (i%3) {
            a += 10;
        } else {
            a -= 100;
        }
    }
    int b = a;
    return b;
    return 0;
}

co_async<> example_coroutine() {
    func();
    co_return;
}

int co_cost_time_test() {
    constexpr int num_iterations = 10000000;

    // 测试普通函数调用
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_iterations; ++i) {
        []() {
            func();
        }(); // 空的 lambda 表达式，模拟函数调用
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Function call time: "
              << double(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / num_iterations
              << " ns per call" << std::endl;

    // 测试协程调用
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_iterations; ++i) {
        example_coroutine().via();
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Coroutine creation time: "
              << double(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / num_iterations
              << " ns per coroutine" << std::endl;

    return 0;
}