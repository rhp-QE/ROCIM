//
// Utility.h
//
// author: Ruan Huipeng
// date : 2025-03-02
// 

#ifndef ROC_BASE_UTIL_UTILITY_H
#define ROC_BASE_UTIL_UTILITY_H

#include <functional>

namespace roc::base::util {

template<typename Re, typename... Args>
Re safe_invoke_block(const std::function<Re(Args...)>& func, Args&&... args) {
    if (func) {
        return func(std::forward<Args>(args)...);
    } 
}

} // namespace roc::base::util

#endif // ROC_BASE_UTIL_UTILITY_H
