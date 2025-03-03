#include "im/base/ProtobufZeroCopyInputStream.h"
#include "im/base/Utility.h"
#include <boost/asio/buffer.hpp>
#include <cstdint>
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

    std::shared_ptr<Request> request = Request::create([](RequestBody *body) {
    });

    request->request([](std::unique_ptr<ResponseBody> response) {
        std::cout<<response->response_id()<<std::endl;
    });


    int value = 0;
    std::function<void(int& value)> func = [](int& value) {
        value += 100;
    };
    
    roc::base::util::safe_invoke_block(func, value);

    value = 0;
}

};