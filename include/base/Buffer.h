/*
 * @Author: Ruan Huipeng 
 * @Date: 2025-01-12 19:01:08 
 * @Last Modified by: Ruan Huipeng
 * @Last Modified time: 2025-01-15 23:43:55
 */

#ifndef ROCIM_CLIENT_BUFFER_H 
#define ROCIM_CLIENT_BUFFER_H

#include <optional>
#include <string>
#include <vector>

namespace roc {
namespace base {

/// copyable
/// not threadSafe

class Buffer {
public:

    std::optional<std::string> readString(int n);

    inline int readableBytes() const;
    
    inline int writeableBytes() const;

    void appendString(const std::string &str);

private:
    int readIndex_;
    int writeIndex_;
    const int leastPrependBytes_ = 8;
    std::vector<char> buffer_;

    inline int prependableBytes() const;

};


}// namespace base
}// namespace roc

#endif