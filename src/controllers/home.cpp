
#include "./home.hpp"

#include <iostream>


namespace Ctrlr

{


void Home::get(const HttpRequestPtr&, std::function<void(const HttpResponsePtr&)>&& cb) {
    std::cout<<"Foo!"<<std::endl;
    drogon::HttpViewData data;
    const auto response = drogon::HttpResponse::newHttpViewResponse("home.csp", data);
    cb(response);
}


}  // namespace Ctrlr

