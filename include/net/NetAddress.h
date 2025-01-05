/*
 * @Author: Ruan Huipeng 
 * @Date: 2025-01-05 00:44:09 
 * @Last Modified by: Ruan Huipeng
 * @Last Modified time: 2025-01-05 20:28:45
 */

#ifndef ROCIM_NetAddress_h
#define ROCIM_NetAddress_h

#include <boost/noncopyable.hpp>
#include <string_view>
#include <sys/types.h>

namespace rocim {
namespace net {

/// 值对象
class NetAddress {
    std::string_view network;
    u_int32_t port;
    std::string_view ip;
};

} /// namespace net
} /// namespace rocim

#endif