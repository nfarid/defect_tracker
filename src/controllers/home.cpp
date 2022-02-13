
#include "./home.hpp"


namespace Ctrlr
{


void Home::index(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb) {
    const auto session = req->session();
    if(!session)
        std::abort();

    const auto userId = session->getOptional<int32_t>("user");
    std::optional<std::string> username{};
    if(userId)
        username = mAccountOrm.findByPrimaryKey(*userId).getValueOfUsername();

    HttpViewData data;
    data.insert("username", username);
    const auto response = HttpResponse::newHttpViewResponse("home.csp", data);
    cb(response);
}


}  // namespace Ctrlr

