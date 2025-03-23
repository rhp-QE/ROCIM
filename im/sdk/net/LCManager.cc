// LCManager.cc
//
// author: Ruan Huipeng
// date : 2025-03-23
//

#include "LCManager.h"
#include "im/sdk/net/UrlTraits.h"
#include <array>
#include <atomic>
#include <cstddef>
#include <im/sdk/model/LCModel.pb.h>
#include <memory>
#include <mutex>
#include <optional>
#include <string>

// 使用命名空间
namespace roc::im::sdk::net {


inline size_t generate_request_id() {
    static std::atomic_int32_t id = 0;
    return (++id);
}

// --------------------  fill traits (begin) ------------------------

template <const char* url>
void fill_request_body(RequestBody *request_body, typename UrlTraits<url>::request_type *params) {}

template <>
void fill_request_body<send_message_url>(RequestBody *request_body, typename UrlTraits<send_message_url>::request_type *params) {
    request_body->set_allocated_send_batch_messages_request(params);
}

template <>
void fill_request_body<fetch_mix_link_messages_url>(RequestBody *request_body, typename UrlTraits<fetch_mix_link_messages_url>::request_type *params) {
    request_body->set_allocated_fetch_mixed_link_messages_request(params);
}

template <>
void fill_request_body<fetch_single_link_messages_url>(RequestBody *request_body, typename UrlTraits<fetch_single_link_messages_url>::request_type *params) {
    request_body->set_allocated_fetch_single_link_messages_request(params);
}

// --------------------  fill traits (end) ------------------------

// --------------------  fill traits (begin) ------------------------

template <const char* url>
const typename UrlTraits<url>::response_type* parse_response_body(ResponseBody *response_body) { return nullptr; }

template <>
const UrlTraits<send_message_url>::response_type* parse_response_body<send_message_url>(ResponseBody *response_body) {
    auto resp = &response_body->send_batch_messages_response();
    response_body->set_allocated_send_batch_messages_response(nullptr);
    return resp;
}

template <>
const UrlTraits<fetch_mix_link_messages_url>::response_type* parse_response_body<fetch_mix_link_messages_url>(ResponseBody *response_body) {
    auto resp = &response_body->fetch_mixed_link_messages_response();
    response_body->set_allocated_fetch_mixed_link_messages_response(nullptr);
    return resp;
}

template <>
const UrlTraits<fetch_single_link_messages_url>::response_type* parse_response_body<fetch_single_link_messages_url>(ResponseBody *response_body) {
    auto resp = &response_body->fetch_single_link_messages_response();
    response_body->set_allocated_fetch_single_link_messages_response(nullptr);
    return resp;
}

template <>
const UrlTraits<push_message_url>::response_type* parse_response_body<push_message_url>(ResponseBody *response_body) {
    auto resp = &response_body->push_message();
    response_body->set_allocated_push_message(nullptr);
    return resp;
}

// --------------------  fill traits (end) ------------------------


template<const char* url>
awaitable<typename UrlTraits<url>::response_type *>
    LCManager::request(typename UrlTraits<url>::response_type* params) {
    
    std::unique_ptr<RequestBody> request_body = std::make_unique<RequestBody>();

    /// 填充request 消息体
    auto id = generate_request_id();
    request_body->set_request_id(std::to_string(id));
    fill_request_body<url>(request_body.get(), params);

    /// 序列化
    size_t len = request_body->ByteSizeLong() ?: 1;
    std::vector<char> buf(len);
    request_body->SerializePartialToArray(buf.data(), len);
    
    /// 数据发送
    auto res = co_await lc_->send(buf.data(), len);

    co_return (co_await await_response_<url>(id));
}


template<const char* url>
awaitable<typename UrlTraits<url>::response_type *> LCManager::await_response_(std::string id) {

    auto getResp = [this, id]() ->std::optional<ResponseBody *>{
        std::optional<ResponseBody *> resp = std::nullopt;
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = response_map_.find(id);
        if (it != response_map_.end()) {
            resp = it->second;
        }
    };

    auto resp = getResp();
    if (resp) {
        co_return resp;
    }

    struct await {
        std::string id;
        std::shared_ptr<LCManager> manager;
        std::function<std::optional<ResponseBody *>()> func;

        bool await_ready() { return false; }
        void await_suspend(std::coroutine_handle<> h) {
            std::lock_guard<std::mutex> lock(manager->mutex_);
            manager->coro_handle_[id] = h;
        }
        std::optional<ResponseBody *> await_resume() { return func(); }
    };

    std::unique_ptr<ResponseBody> response_body = std::make_unique<ResponseBody>(co_await await(id, this, getResp));
    if (response_body) {
        co_return parse_response_body<url>(response_body.get());
    } else {
        co_return nullptr;
    }
}


} // namespace roc::net
