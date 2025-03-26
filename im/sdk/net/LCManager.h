//
// LCManager.h
//
// author: Ruan Huipeng
// date : 2025-03-23
// 

#ifndef ROC_NET_LCMANAGER_H
#define ROC_NET_LCMANAGER_H

#include "im/base/noncopyable.h"
#include "im/sdk/net/UrlTraits.h"
#include <boost/asio/awaitable.hpp>
#include <boost/asio/experimental/channel.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/system/detail/error_code.hpp>
#include <coroutine>
#include <im/base/lc/impl/ws/WSConnection.h>
#include <memory>
#include <unordered_map>

// ------------- pre declare (begin) -----------------

namespace roc::base::net {

template <typename T>
class ICoroLongConnection;

}// namespace

// ------------- pre declare (end) -----------------



namespace roc::im::sdk::net {

class ResponseBody;
class PushMessages;

using namespace boost::asio;

struct LCManagerConfig {
    std::string host;
    std::string port;
};
// connect -> run 
class LCManager : public noncopyable, public std::enable_shared_from_this<LCManager> {

public:
    explicit LCManager(LCManagerConfig config, io_context::executor_type executor);

    // 发起请求
    // params request 内部会进行释放
    template<const char* url>
    awaitable<std::unique_ptr<typename UrlTraits<url>::response_type *>>
        request(std::unique_ptr<typename UrlTraits<url>::response_type> params);

    awaitable<bool> connect();

    // 注册处理 下推消息
    void handlePushMessages(std::function<void(PushMessages* PushMessages)>);

private:
    using lc_type = base::net::ICoroLongConnection<base::net::WSConnection>;
    using ws_type = base::net::WSConnection;
    
    bool isRunning_ = false;
    LCManagerConfig config_;
    io_context::executor_type executor_;
    std::shared_ptr<lc_type> lc_ = std::make_shared<ws_type>();

    using channel_type = experimental::channel<void(boost::system::error_code, std::unique_ptr<ResponseBody>)>;
    std::unordered_map<std::string, std::unique_ptr<channel_type>> channel_map_;

    std::function<void(const PushMessages* PushMessages)> handlePushMessagesCallback_;

    awaitable<void> do_read_();
};

} // namespace roc::im::sdk::net
#endif // ROC_NET_LCMANAGER_H
