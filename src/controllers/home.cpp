
#include "./home.hpp"


namespace Ctrlr
{


void Home::index(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb) {
    const auto session = req->session();
    if(!session)
        std::abort();

    const auto user_id = session->getOptional<int32_t>("user");
    std::optional<std::string> username{};
    if(user_id)
        username = m_account_orm.findByPrimaryKey(*user_id).getValueOfUsername();

    HttpViewData data;
    data.insert("username", username);
    const auto response = HttpResponse::newHttpViewResponse("home.csp", data);
    cb(response);
}


}  // namespace Ctrlr

