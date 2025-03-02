// Request.cc
//
// author: Ruan Huipeng
// date : 2025-02-28
//


#include <memory>

#include <im/sdk/net/Request.h>
#include <im/base/Utility.h>
#include <im/sdk/pb/im.pb.h>

// 使用命名空间
namespace roc::im::sdk::net {


std::shared_ptr<Request> Request::create(std::function<void (RequestBody *)> initBlock) {
    std::shared_ptr<Request> request = std::make_shared<Request>();

    std::unique_ptr<RequestBody> request_body = std::make_unique<RequestBody>();
    roc::base::util::safe_invoke_block(initBlock, request_body.get());

    return request;
}


std::shared_ptr<Request> Request::request(std::function<void(std::unique_ptr<ResponseBody>)> callback) {
    auto self = shared_from_this();

    return self;
}

} // namespace roc::im::sdk::net
