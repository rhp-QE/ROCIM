//
// ConnectionManager.h
//
// author: Ruan Huipeng
// date : 2025-02-28
// 

#ifndef ROC_IM_SDK_NET_CONNECTIONMANAGER_H
#define ROC_IM_SDK_NET_CONNECTIONMANAGER_H


#include <boost/core/noncopyable.hpp>
#include <boost/noncopyable.hpp>
#include <memory>

namespace roc::im::sdk::net {

class ConnectionManager : public boost::noncopyable, std::enable_shared_from_this<ConnectionManager>{
public:
    // 构造函数
    explicit ConnectionManager(int value);

    // 拷贝构造函数
    ConnectionManager(const ConnectionManager& other);

    // 移动构造函数
    ConnectionManager(ConnectionManager&& other) noexcept;

    // 拷贝赋值操作符（使用拷贝-交换技术）
    ConnectionManager& operator=(ConnectionManager other);

    // 移动赋值操作符
    ConnectionManager& operator=(ConnectionManager&& other) noexcept;

    // 成员函数
    void setValue(int value);
    int getValue() const;

private:
    int value; // 成员变量
};

} // namespace roc::im::sdk::net

#endif // ROC_IM_SDK_NET_CONNECTIONMANAGER_H
