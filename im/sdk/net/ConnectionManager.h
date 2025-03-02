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

class ConnectionManager : public boost::noncopyable, public std::enable_shared_from_this<ConnectionManager>{
public:
    static ConnectionManager& shareInstance() {
        static ConnectionManager instance;
        return instance;
    }

private:
    ConnectionManager();
    ~ConnectionManager() = default;
};

} // namespace roc::im::sdk::net

#endif // ROC_IM_SDK_NET_CONNECTIONMANAGER_H
