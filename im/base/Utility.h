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

template<typename Func, typename... Args>
void safe_invoke_block(const std::function<Func>& func, Args&&... args) {
    if (func) {
        func(std::forward<Args>(args)...);
    } 
}

} // namespace roc::base::util

#endif // ROC_BASE_UTIL_UTILITY_H
