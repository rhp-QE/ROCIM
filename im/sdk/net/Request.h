//
// Request.h
//
// author: Ruan Huipeng
// date : 2025-02-28
// 

#ifndef ROC_IM_SDK_NET_REQUEST_H
#define ROC_IM_SDK_NET_REQUEST_H

#include "im/sdk/pb/im.pb.h"
#include <boost/noncopyable.hpp>
#include <boost/system/system_error.hpp>
#include <memory>


namespace roc::im::sdk::net {

// 前置声明
class ResponseBody;
class RequestBody;
class ConnectionManager;

class Request : public boost::noncopyable, public std::enable_shared_from_this<Request> {
    friend ConnectionManager;

public:

    static std::shared_ptr<Request> create(std::function<void(RequestBody *body)> block);
    std::shared_ptr<Request> request(std::function<void(std::unique_ptr<ResponseBody>)> callback);

private:
    std::unique_ptr<RequestBody> request_body_;
    std::function<void(std::unique_ptr<ResponseBody>)> callback_;
};

} // namespace roc::im::sdk::net

#endif // ROC_IM_SDK_NET_REQUEST_H
