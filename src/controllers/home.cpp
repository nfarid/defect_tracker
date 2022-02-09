
#include "./home.hpp"

#include <iostream>


namespace Ctrlr

{


void Home::get(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb) {
    std::cout<<"Home::get:"<<std::endl;

    const auto session = req->session();
    if(!session)
        std::abort();

    const auto user_id = session->getOptional<int32_t>("user");
    std::optional<std::string> username{};
    if(user_id)
        username = m_account_orm.findByPrimaryKey(*user_id).getValueOfUsername();

    std::cout<<(username ? *username : "NULL")<<std::endl;

    drogon::HttpViewData data;
    data.insert("username", username);
    const auto response = drogon::HttpResponse::newHttpViewResponse("home.csp", data);
    cb(response);
}


}  // namespace Ctrlr

