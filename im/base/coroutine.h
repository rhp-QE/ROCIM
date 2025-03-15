#ifndef ROC_COROUTINE_H
#define ROC_COROUTINE_H


#include <algorithm>
#include <boost/core/noncopyable.hpp>
#include <coroutine>
#include <future>
#include <iostream>
#include <memory>
#include <optional>
#include <unordered_map>
#include <im/base/noncopyable.h>

namespace roc::coro {

// ===================== std:coroutine RAII =====================

class CoroRAII {
    using handle_type = std::coroutine_handle<>;
public:
    CoroRAII(handle_type coro_handle) : coro_handle_(coro_handle) {}
    ~CoroRAII() {
        if (coro_handle_) { coro_handle_.destroy(); }
    }

    handle_type coro_handle() { return coro_handle_; }
private: 
    handle_type coro_handle_;
};

//------------------------ std::coroutine RAII ---------------------

//-------------------   协程生命周期管理  -----------------------------
extern std::unordered_map<size_t, std::shared_ptr<CoroRAII>> coro_manager;
inline size_t ref_coro(std::shared_ptr<CoroRAII> coro) {
    static size_t id = 0;
    ++id;
    coro_manager[id] = coro;
    return id; 
}

inline void unref_coro(size_t id) {
    coro_manager.erase(id);
}
//-------------------------------------------------------------------



// ==================  co_async 封装  (存在返回值)   ===================
template<typename ReType = void>
struct co_async {
    std::shared_ptr<CoroRAII> coro_arii_sptr;

public:
    struct promise_type;
    struct awaiter;

    co_async(std::shared_ptr<CoroRAII> h) : coro_arii_sptr(h) { }

    auto operator co_await() noexcept {
        return awaiter{ 
            std::coroutine_handle<promise_type>::from_address(coro_arii_sptr->coro_handle().address())
        };
    }

    // =================== awaiter ================
    struct awaiter {
        std::coroutine_handle<promise_type>  coro_handle;

        bool await_ready() const noexcept {
            return !coro_handle || coro_handle.done();
        }

        void await_suspend(std::coroutine_handle<> awaiting_coro) noexcept {
            coro_handle.promise().parent = awaiting_coro;
        }

        ReType await_resume() {
            return coro_handle.promise().result.value();
        }
    };
    // ----------------- awaiter --------------------

    // ================  promise_type   =============
    struct promise_type {
    
    private:
        size_t id;

    public:
        std::optional<ReType> result;   // 协程的返回值
        std::coroutine_handle<> parent; // 保存父协程句柄

        co_async get_return_object() {
            auto h = std::coroutine_handle<promise_type>::from_promise(*this);
            auto handle =  std::make_shared<CoroRAII>(std::coroutine_handle<>::from_address(h.address()));
            id = ref_coro(handle);

            return co_async{ handle };
        }

        std::suspend_never initial_suspend() noexcept { return {}; }

        auto final_suspend() noexcept {
            struct FinalAwaiter {
                bool await_ready() noexcept {  return false; }
                void await_suspend(std::coroutine_handle<promise_type> self) noexcept {
                    unref_coro(self.promise().id);
                    if (self.promise().parent) {
                        self.promise().parent.resume(); // 恢复父协程
                    }
                }
                void await_resume() noexcept {}
            };
            return FinalAwaiter{};
        }

        void return_value(ReType v) noexcept { result = v; }

        void unhandled_exception() { std::terminate(); }
    };
    // ------------------------ promise_type --------------------

};
// ------------------------ co_async 封装 ------------------------


// ==================  co_async 封装  (返回值为空)   ==============
template<>
struct co_async<void> {

    std::shared_ptr<CoroRAII> coro_arii_sptr;

public:
    struct promise_type;
    struct awaiter;

    co_async(std::shared_ptr<CoroRAII> h) : coro_arii_sptr(h) { }

    auto operator co_await() noexcept {
        return awaiter{ 
            std::coroutine_handle<promise_type>::from_address(coro_arii_sptr->coro_handle().address())
        };
    }

    // =================== awaiter ================
    struct awaiter {
        std::coroutine_handle<promise_type> coro_handle;

        bool await_ready() const noexcept {
            return !coro_handle || coro_handle.done();
        }

        void await_suspend(std::coroutine_handle<> awaiting_coro) noexcept {
            coro_handle.promise().parent = awaiting_coro;
        }

        void await_resume() {}
    };
    // ----------------- awaiter --------------------

    // ================  promise_type   =============
    struct promise_type {
    
    private:
        size_t id;

    public:
        std::coroutine_handle<> parent; // 保存父协程句柄

        co_async get_return_object() {
            auto h = std::coroutine_handle<promise_type>::from_promise(*this);
            auto handle =  std::make_shared<CoroRAII>(std::coroutine_handle<>::from_address(h.address()));
            id = ref_coro(handle);

            return co_async{ handle };
        }

        std::suspend_never initial_suspend() noexcept { return {}; }

        auto final_suspend() noexcept {
            struct FinalAwaiter {
                bool await_ready() noexcept {  return false; }
                void await_suspend(std::coroutine_handle<promise_type> self) noexcept {
                    unref_coro(self.promise().id);
                    if (self.promise().parent) {
                        self.promise().parent.resume(); // 恢复父协程
                    }
                }
                void await_resume() noexcept {}
            };
            return FinalAwaiter{};
        }

        void return_void() {}

        void unhandled_exception() { std::terminate(); }
    };
    // ------------------------ promise_type --------------------

};
// ------------------------ co_async 封装 ------------------------



// ===== 封装 awaitable_wapper (便于将异步回调函数封装成awaitable) =====
template <typename Ty = void>
struct co_awaitable_wapper;

template <typename Ty = void>
class CoroPromise;

// -------- coroutine promise ------------
template <typename Ty>
class CoroPromise{
    friend co_awaitable_wapper<Ty>;

public:
    void set_value(const Ty& value) {
        promise_.set_value(value);
        h.resume();
    }

private:
    std::promise<Ty> promise_;
    std::coroutine_handle<> h;
};

// 偏特化版本
// -------- coroutine promise ------------
template <>
class CoroPromise<void>{
    friend co_awaitable_wapper<void>;

public:
    void set_value() {
        promise_.set_value();
        h.resume();
    }

private:
    std::promise<void> promise_;
    std::coroutine_handle<> h;
};


template <typename ReType>
class co_awaitable_wapper {

    using Type = std::function<void(std::shared_ptr<CoroPromise<ReType>> promise)>;

    std::future<ReType> future;
    Type func;

public:
    bool await_ready() const noexcept { return false; }

    void await_suspend(std::coroutine_handle<> h) {
        CoroPromise<ReType> co_promise;
        co_promise.h = h;
        co_promise.promise_ = std::promise<ReType>();
        future = co_promise.promise_.get_future();

        func(std::make_shared<CoroPromise<ReType>>(std::move(co_promise)));
    }

    ReType await_resume() noexcept { return future.get(); }

    co_awaitable_wapper(Type fun) : func(fun) {}
};

// --------------------- 封装 await_able_wapper ---------------------


} // namespace roc::coro

#endif