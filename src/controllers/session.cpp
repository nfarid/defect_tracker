
#include "./session.hpp"

#include <iostream>


namespace Ctrlr
{


void Session::newGet(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb) {
    const auto session = req->session();
    if(!session)
        std::abort();
    const auto userId = session->getOptional<int32_t>("user");

    // If the user has already logged in, then there's no point of the login page.
    if(userId) {
        const auto res = HttpResponse::newRedirectionResponse("/");
        cb(res);
        return;
    }

    std::optional<std::string> username{};
    HttpViewData data;
    data.insert("username", username);
    const auto response = HttpResponse::newHttpViewResponse("login.csp", data);
    cb(response);
}


}  // namespace Ctrlr

