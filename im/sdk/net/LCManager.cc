// LCManager.cc
//
// author: Ruan Huipeng
// date : 2025-03-23
//

#include "LCManager.h"
#include "im/sdk/net/UrlTraits.h"
#include <array>
#include <atomic>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/buffers_iterator.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/this_coro.hpp>
#include <boost/beast/websocket/impl/write.hpp>
#include <cassert>
#include <cstddef>
#include <im/sdk/model/LCModel.pb.h>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <im/base/lc/Interface/ICoroLongConnection.h>

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
    return response_body->unsafe_arena_release_send_batch_messages_response();
}

template <>
const UrlTraits<fetch_mix_link_messages_url>::response_type* parse_response_body<fetch_mix_link_messages_url>(ResponseBody *response_body) {
    return response_body->unsafe_arena_release_fetch_mixed_link_messages_response();
}

template <>
const UrlTraits<fetch_single_link_messages_url>::response_type* parse_response_body<fetch_single_link_messages_url>(ResponseBody *response_body) {
    return response_body->unsafe_arena_release_fetch_single_link_messages_response();
}

template <>
const UrlTraits<push_message_url>::response_type* parse_response_body<push_message_url>(ResponseBody *response_body) {
    return response_body->unsafe_arena_release_push_messages();
}

// --------------------  fill traits (end) ------------------------

LCManager::LCManager(LCManagerConfig config, io_context::executor_type executor) :
    config_(config),
    executor_(executor) 
    {}

template<const char* url>
awaitable<std::unique_ptr<typename UrlTraits<url>::response_type *>>
    LCManager::request(std::unique_ptr<typename UrlTraits<url>::response_type> params) {
    
    auto current_ex = co_await this_coro::executor;

    // 切换到 io excutor
    co_await post(bind_executor(executor_, use_awaitable));

    /// 填充request 消息体
    std::unique_ptr<RequestBody> request_body = std::make_unique<RequestBody>();
    std::string request_id = std::to_string(generate_request_id());
    request_body->set_request_id(request_id);
    fill_request_body<url>(request_body.get(), params.release());

    /// 序列化
    size_t len = request_body->ByteSizeLong() ?: 1;
    std::vector<char> buf(len);
    request_body->SerializePartialToArray(buf.data(), len);
    
    /// 数据发送
    auto send_ok = co_await lc_->send(buf.data(), len);

    /// 等待结果
    channel_map_[request_id] = std::make_unique<channel_type>(executor_, 1);
    std::unique_ptr<ResponseBody> response_body = co_await channel_map_[request_id]->async_receive();

    auto it = std::move(channel_map_[request_id]);
    channel_map_.erase(request_id);
    it->close();

    /// resp 从 response_body 中解析出来
    using response_type = typename UrlTraits<url>::response_type;
    std::unique_ptr<response_type> resp = std::make_unique<response_type>(parse_response_body<url>(response_body.get()));
    
    // 切换回调用 excutor
    co_await post(bind_executor(current_ex, use_awaitable));

    co_return resp;
}

awaitable<bool> LCManager::connect() {
    auto current_ex = co_await this_coro::executor;

    // 切换到 io excutor
    co_await post(bind_executor(executor_, use_awaitable));

    // 发起连接
    auto connected = co_await lc_->connect({config_.host, config_.port});

    // 持续接受数据
    co_spawn(executor_, do_read_(), detached);

    // 切换回调用 excutor
    co_await post(bind_executor(current_ex, use_awaitable));

    co_return connected;
}


awaitable<void> LCManager::do_read_() {
    do{
        boost::beast::flat_buffer receive_data = co_await lc_->read();
        // 1、数据解析
        std::unique_ptr<ResponseBody> response_body = std::make_unique<ResponseBody>();
        response_body->ParseFromArray(receive_data.data().data(), receive_data.size());
        
        // 2、判断是否是 push message
        if (response_body->has_push_messages()) {
            const PushMessages* push_messages = response_body->unsafe_arena_release_push_messages();
            handlePushMessagesCallback_(push_messages);
            continue;
        }

        // 3、判断是否是 response
        std::string response_id = response_body->response_id();
        auto it = channel_map_.find(response_id);
        if (it != channel_map_.end()) {
            co_await (it->second)->async_send({}, std::move(response_body));
        }
    } while(true);
    co_return;
}

} // namespace roc::net
