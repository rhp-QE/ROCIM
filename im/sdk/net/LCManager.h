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

using namespace boost::asio;

struct LCManagerConfig {
    std::string host;
    std::string port;
};

class LCManager : public noncopyable, public std::enable_shared_from_this<LCManager> {


public:
    explicit LCManager(LCManagerConfig config);

    template<const char* url>
    awaitable<typename UrlTraits<url>::response_type *>
        request(typename UrlTraits<url>::response_type* params);

private:
    using lc_type = base::net::ICoroLongConnection<base::net::WSConnection>;
    using ws_type = base::net::WSConnection;
    
    std::mutex mutex_;
    std::shared_ptr<lc_type> lc_ = std::make_shared<ws_type>();
    std::unordered_map<std::string, ResponseBody *> response_map_;
    std::unordered_map<std::string, std::coroutine_handle<>> coro_handle_;

    template<const char* url>
    awaitable<typename UrlTraits<url>::response_type *> await_response_(std::string id);
};

} // namespace roc::im::sdk::net

#endif // ROC_NET_LCMANAGER_H
