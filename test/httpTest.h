
#include <boost/json.hpp>
#include <boost/json/array.hpp>
#include <boost/json/object.hpp>
#include <boost/json/parse.hpp>
#include <boost/json/serialize.hpp>
#include <iostream>



int testHttp() {
    using namespace::roc::net;

    boost::json::object json_sub1;
    json_sub1["name"] = "rxy";
    json_sub1["age"] = 12;

    boost::json::object json_sub;
    json_sub["name"] = "rxy";
    json_sub["age"] = 12;
    json_sub["lick"] = boost::json::array{90,100,100, 89, json_sub1};

    boost::json::object json;
    json["name"] = "rhp";
    json["age"] = 21;
    json["sister"] = json_sub;

    Request request;
    request
    .set_url("/echo")
    .set_method(Request::Method::POST)
    .set_body(boost::json::serialize(json))
    .set_callback([=](std::string str) {
        std::cout<<str<<std::endl;
    })
    .request();


    return 0;
}