#include <memory>
#include <iostream>

#include <im/sdk/net/Request.h>
#include <im/sdk/pb/im.pb.h>

namespace roc::im::sdk::net {

// 测试
void test() {
    std::cout<<"test in"<<std::endl;
    std::shared_ptr<Request> request = Request::create([](RequestBody *body) {
        std::cout<<"create request"<<std::endl;

        int a = 100;
        int b = 100;
        int c = a + b;
    });


    request->request([](std::unique_ptr<ResponseBody> response) {

    });
}

};