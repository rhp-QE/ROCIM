#include <coroutine>
#include <iostream>
#include <functional>
#include <memory>
#include <thread>
#include <chrono>

// 异步操作：延迟执行回调
void async_timer(int ms, std::function<void()> callback) {
    std::thread([ms, callback] {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        callback(); // 异步操作完成后调用回调
    }).detach();
}

using Type = std::function<void(std::coroutine_handle<> h)>;

// 自定义 Awaitable 类型
struct TimerAwaitable {
    int delay_ms;

    bool await_ready() const noexcept { 
        return false; // 总是挂起协程
    }

    void await_suspend(std::coroutine_handle<> h) const {
        func(h);
    }

    void await_resume() const noexcept {}

    Type func;

    TimerAwaitable(Type fun) : func(fun) {}
};

struct AsyncTask {
    struct promise_type {
        AsyncTask get_return_object() { 
            return AsyncTask{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_never initial_suspend() { return {}; } // 立即启动协程
        std::suspend_always final_suspend() noexcept { return {}; }
        void unhandled_exception() { std::terminate(); }
        void return_void() {}
    };

    std::coroutine_handle<promise_type> handle;
    explicit AsyncTask(std::coroutine_handle<promise_type> h) : handle(h) {}
    ~AsyncTask() { if (handle && handle.done()) handle.destroy(); }
};


void request1(std::coroutine_handle<> h) {
    std::cout<<"request 1"<<std::endl;
    async_timer(5000, [h]() mutable {
        std::cout<<"response 1"<<std::endl;
        h.resume(); // 定时器完成后恢复协程
    });
}


void request2(std::coroutine_handle<> h) {
    std::cout<<"request 2"<<std::endl;
    async_timer(5000, [h]() mutable {
        std::cout<<"response 2"<<std::endl;
        h.resume(); // 定时器完成后恢复协程
    });
}


inline void test() {
    //AsyncTask task = my_coroutine(); // 协程立即启动
    
    []() -> AsyncTask {
        std::cout << "协程开始"<< std::this_thread::get_id()<<std::endl;
    
            // 启动异步定时器（挂起协程，等待2秒）
        co_await TimerAwaitable{[](std::coroutine_handle<> h) {
            request1(h);
        }};

        co_await TimerAwaitable{[](std::coroutine_handle<> h) {
            request2(h);
        }};
    
        std::cout << "异步操作完成，协程恢复"<<std::this_thread::get_id()<<std::endl;
    }();

    std::cout << "主线程继续...\n";
}