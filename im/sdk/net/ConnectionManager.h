//
// ConnectionManager.h
//
// author: Ruan Huipeng
// date : 2025-02-28
// 

#ifndef ROC_IM_SDK_NET_CONNECTIONMANAGER_H
#define ROC_IM_SDK_NET_CONNECTIONMANAGER_H


#include "im/base/ILongConnection.h"
#include "im/base/LongConnectionImpl.h"
#include "im/sdk/net/Request.h"
#include <boost/core/noncopyable.hpp>
#include <boost/noncopyable.hpp>
#include <memory>
#include <unordered_map>

namespace roc::im::sdk::net {

using Default_LC_Type = roc::net::ILongConnection<roc::net::LongConnectionImpl>;

class ConnectionManager : public boost::noncopyable, public std::enable_shared_from_this<ConnectionManager>{
public:
    static ConnectionManager& shareInstance() {
        static ConnectionManager instance;
        return instance;
    }

    void send(std::shared_ptr<Request> request);

    /// sdk 专用长链
    std::shared_ptr<Default_LC_Type> default_lc() { return default_lc_; }

private:
    std::shared_ptr<Default_LC_Type> default_lc_;
    std::unordered_map<std::string, std::shared_ptr<Request>> requestRecord;

    size_t id_ = 0;

    ConnectionManager();
    ~ConnectionManager() = default;
};

} // namespace roc::im::sdk::net

#endif // ROC_IM_SDK_NET_CONNECTIONMANAGER_H
