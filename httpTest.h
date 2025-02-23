#include <boost/asio/executor_work_guard.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <thread>
#include <boost/json.hpp>

#include <base/HttpClient.h>


int testHttp() {
    

    for (int i = 0; i < 100; ++i) {
        roc::net::Reuqest request;
        request
        .set_url("/hello")
        .set_method("GET")
        .set_callback([=](std::string str) {
            std::cout<<str<<" "<<i<<std::endl;
        })
        .request();
    }

    return 0;
}