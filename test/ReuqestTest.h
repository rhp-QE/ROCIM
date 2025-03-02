#include <memory>
#include <iostream>


#include <sdk_test/ReuqestTest.h>

namespace roc::im::sdk::net {

// 测试
void test() {

    std::shared_ptr<Request> request = Request::create([](RequestBody *body) {
        std::cout<<"create request";
    });


    request->request([](std::unique_ptr<ResponseBody> response) {

    });
}

};