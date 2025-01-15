/*
 * @Author: Ruan Huipeng 
 * @Date: 2025-01-05 20:30:56 
 * @Last Modified by: Ruan Huipeng
 * @Last Modified time: 2025-01-15 23:45:32
 */

#ifndef ROCIM_CLIENT_NET_LONGCONNECTION
#define ROCIM_CLIENT_NET_LONGCONNECTION

#include "base/Buffer.h"
#include <boost/core/noncopyable.hpp>
#include <functional>

/// pre declare ------------------
class Buffer;
class NetAddress;
/// ------------------------------

namespace rocim {
namespace net {


using ReceiveDataCallBackType = std::function<void()>;

class LongConnection : boost::noncopyable {
    explicit LongConnection(NetAddress netAddress);
    void connect();
    void asyncSend();
    void addReciveDataCallback();
};

}
}

#endif
