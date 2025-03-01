//
// Request.h
//
// author: Ruan Huipeng
// date : 2025-02-28
// 

#ifndef ROC_IM_SDK_NET_REQUEST_H
#define ROC_IM_SDK_NET_REQUEST_H

#include <boost/noncopyable.hpp>
#include <boost/system/system_error.hpp>
#include <memory>


namespace roc::im::sdk::net {

// 前置声明
class ResponseBody;
class RequestBody;

class Request : public boost::noncopyable, std::enable_shared_from_this<Request> {
public:
    // 构造函数
    explicit Request(int value);

    // 移动构造函数
    Request(Request&& other) noexcept;

    // 移动赋值操作符
    Request& operator=(Request&& other) noexcept;

    // 成员函数
    void setValue(int value);
    int getValue() const;

    static std::shared_ptr<Request> 

private:
    int value; // 成员变量
};

} // namespace roc::im::sdk::net

#endif // ROC_IM_SDK_NET_REQUEST_H
