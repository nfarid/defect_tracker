
#include "./home.hpp"

#include <string>
#include <iostream>


namespace Ctrlr
{


using std::string_literals::operator""s;


void Home::index(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb) {
    const auto session = req->session();
    if(!session) {
        std::cerr<<"Session is not enabled"<<std::endl;
        throw std::runtime_error("Session is not enabled");
    }

    const auto userId = session->getOptional<int32_t>("user");
    std::optional<std::string> username{};
    if(userId)
        username = mAccountOrm.findByPrimaryKey(*userId).getValueOfUsername();

    HttpViewData data;
    data.insert("title", "Home"s);
    data.insert("username", username);
    const auto response = HttpResponse::newHttpViewResponse("home.csp", data);
    cb(response);
}


}  // namespace Ctrlr

