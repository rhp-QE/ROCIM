#include "im/base/ProtobufZeroCopyInputStream.h"
#include "im/base/Utility.h"
#include "im/base/coroutine.h"
#include <boost/asio/buffer.hpp>
#include <cstdint>
#include <google/protobuf/io/zero_copy_stream.h>
#include <memory>
#include <iostream>

#include <im/sdk/net/Request.h>
#include <im/sdk/pb/im.pb.h>

#include <im/base/LinkBuffer.h>
#include <im/base/ProtobufZeroCopyOutStream.h>

using namespace roc::coro;

namespace roc::im::sdk::net {

co_async<> requestSingle() {
    std::shared_ptr<Request> request = Request::create([](RequestBody *body) {
    });

    auto res = co_await request->async_request();

    std::cout<<"\n resposne id] "<<res->response_id()<<std::endl;
    co_return;
}

// 测试
void testProtobuf() {

    int value = 0;
    for (int i = 0; i < 10; ++i) {
        requestSingle();
    }

    return;
}

};