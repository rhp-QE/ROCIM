#include "im/base/ProtobufZeroCopyInputStream.h"
#include <boost/asio/buffer.hpp>
#include <google/protobuf/io/zero_copy_stream.h>
#include <memory>
#include <iostream>

#include <im/sdk/net/Request.h>
#include <im/sdk/pb/im.pb.h>

#include <im/base/LinkBuffer.h>
#include <im/base/ProtobufZeroCopyOutStream.h>

namespace roc::im::sdk::net {

// 测试
void testProtobuf() {
    std::cout<<"test in"<<std::endl;
    std::shared_ptr<Request> request = Request::create([](RequestBody *body) {
        std::cout<<body->ByteSizeLong()<<std::endl;

        body->set_request_id("1234567890");



        std::shared_ptr<roc::base::LinkBuffer> buffer = std::make_shared<roc::base::LinkBuffer>();
        
        size_t size = body->ByteSizeLong();

        buffer->sync_write(size, [body, size](std::vector<boost::asio::mutable_buffer> buf) -> size_t {
            roc::base::io::ProtobufZeroCopyOutStream stream(buf);
            body->SerializePartialToZeroCopyStream(&stream);
            return size;
        });


        auto readBuf = buffer->get_read_buffers();
        roc::base::io::ProtobufZeroCopyInputStream streamInput(readBuf->buffers);
        
        RequestBody requestBodyRe;
        requestBodyRe.ParseFromBoundedZeroCopyStream(&streamInput, 12);
        readBuf->release();

        std::cout<<"obj size" <<requestBodyRe.request_id()<<std::endl;

    });


    request->request([](std::unique_ptr<ResponseBody> response) {
        response->fetch_mixed_link_messages_response();
    });
}

};