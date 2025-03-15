// ConnectionManager.cc
//
// author: Ruan Huipeng
// date : 2025-02-28
//
#include "ConnectionManager.h"
#include "BaseConfig.h"
#include "im/base/ILongConnection.h"
#include "im/base/LinkBuffer.h"
#include "im/base/ProtobufZeroCopyInputStream.h"
#include "im/base/ProtobufZeroCopyOutStream.h"
#include "im/base/Utility.h"
#include "im/sdk/pb/im.pb.h"
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

// 使用命名空间
namespace roc::im::sdk::net {

// 显式构造函数实现
ConnectionManager::ConnectionManager() {
    {
        roc::net::LongConnectionConfig config = {"127.0.0.1", "8282", 30000 };
    
        default_lc_ = std::make_shared<roc::net::LongConnectionImpl>(net_io_context, config);
        
        default_lc_->set_receive_callback([this](
            std::shared_ptr<Default_LC_Type> conn,
            base::LinkBuffer::ReadResult readResult) {
                std::shared_ptr<ResponseBody> response_body = std::make_shared<ResponseBody>();

                roc::base::io::ProtobufZeroCopyInputStream stream(readResult.buffers);
                response_body->ParseFromZeroCopyStream(&stream);

                auto it = requestRecord.find(response_body->response_id());
                if (it == requestRecord.end()) {
                    return;
                }

                roc::base::util::safe_invoke_block((*it).second->callback_,response_body);
                requestRecord.erase(it);
        });

        default_lc_->connect();
    }
}


void ConnectionManager::send(std::shared_ptr<Request> request) {
    std::string request_id = std::to_string(++id_);
    request->request_body_->set_request_id(request_id);

    requestRecord[request_id] = request;

    size_t len = request->request_body_->ByteSizeLong();
    default_lc_->send(len, [request](std::vector<boost::asio::mutable_buffer> bufs){
        roc::base::io::ProtobufZeroCopyOutStream stream(bufs);
        request->request_body_->SerializeToZeroCopyStream(&stream);
    });  
}


} // namespace roc::im::sdk::net
