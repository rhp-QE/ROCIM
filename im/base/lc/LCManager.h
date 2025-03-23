//
// LCManager.h
//
// author: Ruan Huipeng
// date : 2025-03-23
// 

#ifndef ROC_NET_LCMANAGER_H
#define ROC_NET_LCMANAGER_H

namespace roc::net {

class LCManager {
public:
    // 构造函数
    explicit LCManager(int value);

    // 拷贝构造函数
    LCManager(const LCManager& other);

    // 移动构造函数
    LCManager(LCManager&& other) noexcept;

    // 拷贝赋值操作符（使用拷贝-交换技术）
    LCManager& operator=(LCManager other);

    // 移动赋值操作符
    LCManager& operator=(LCManager&& other) noexcept;

    // 成员函数
    void setValue(int value);
    int getValue() const;

private:
    int value; // 成员变量
};

} // namespace roc::net

#endif // ROC_NET_LCMANAGER_H
