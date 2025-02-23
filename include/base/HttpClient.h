#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H


#include <functional>
#include <string>

namespace roc::net {

class Session;

struct Reuqest {

    friend class Session;

    Reuqest& set_url(std::string url) {
        url_ = url;
        return *this;
    }
    Reuqest& set_method(std::string method) {
        method_ = method;
        return *this;
    }
    Reuqest& set_params(std::string params) {
        params_ = params;
        return *this;
    }
    Reuqest& set_callback(std::function<void(std::string data)> callback) {
        callback_ = callback;
        return *this;
    }

    void request();

private:
    std::string url_;
    std::string method_;
    std::string params_;
    std::string host_ = "127.0.0.1";
    std::string port_ = "8080";
    std::function<void(std::string data)> callback_;
};

}


#endif