// ConnectionManager.cc
//
// author: Ruan Huipeng
// date : 2025-02-28
//

#include "ConnectionManager.h"

// 使用命名空间
namespace roc::im::sdk::net {

// 显式构造函数实现
ConnectionManager::ConnectionManager(int value) : value(value) {}

// 拷贝构造函数实现
ConnectionManager::ConnectionManager(const ConnectionManager& other) : value(other.value) {}

// 移动构造函数实现
ConnectionManager::ConnectionManager(ConnectionManager&& other) noexcept : value(other.value) {
    other.value = 0; // 将其他对象的值置为默认值
}

// 拷贝赋值操作符实现（拷贝-交换技术）
ConnectionManager& ConnectionManager::operator=(ConnectionManager other) {
    value = other.value; // 交换值
    return *this; // 返回当前对象
}

// 移动赋值操作符实现
ConnectionManager& ConnectionManager::operator=(ConnectionManager&& other) noexcept {
    if (this == &other) {
        return *this; // 自我赋值检查
    }

    value = other.value; // 移动值
    other.value = 0; // 将其他对象的值置为默认值
    return *this;
}

// 设置值的函数实现
void ConnectionManager::setValue(int value) {
    this->value = value;
}

// 获取值的函数实现
int ConnectionManager::getValue() const {
    return value;
}

} // namespace roc::im::sdk::net
