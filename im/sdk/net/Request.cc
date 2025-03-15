// Request.cc
//
// author: Ruan Huipeng
// date : 2025-02-28
//


#include <boost/asio/buffer.hpp>
#include <functional>
#include <memory>

#include <im/sdk/net/Request.h>
#include <im/base/Utility.h>
#include <im/sdk/pb/im.pb.h>

#include <im/sdk/net/ConnectionManager.h>
#include <im/base/coroutine.h>

// 使用命名空间
namespace roc::im::sdk::net {


std::shared_ptr<Request> Request::create(std::function<void(RequestBody *)> initBlock) {
    std::shared_ptr<Request> request = std::make_shared<Request>();

    request->request_body_ = std::make_unique<RequestBody>();
    roc::base::util::safe_invoke_block(initBlock, request->request_body_.get());

    return request;
}


std::shared_ptr<Request> Request::request(std::function<void(std::shared_ptr<ResponseBody>)> callback) {
    auto self = shared_from_this();
    self->callback_ = callback;
    ConnectionManager::shareInstance().send(self);
    return self;
}

roc::coro::co_async<std::shared_ptr<ResponseBody>> Request::async_request() {
    using namespace roc::coro;
    using ReType = std::shared_ptr<ResponseBody>;
    auto self = shared_from_this();

    auto res = co_await co_awaitable_wapper<ReType>([self](CoroPromise<ReType> promise){
        self->request([promise](ReType resp) mutable {
            promise.set_value(resp);
        });
    });

    co_return res;
}


} // namespace roc::im::sdk::net
