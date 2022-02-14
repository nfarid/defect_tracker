
#include "./session.hpp"

#include <iostream>


namespace Ctrlr
{


void Session::newGet(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb) {
    const auto session = req->session();
    if(!session)
        std::abort();
    const auto userId = session->getOptional<int32_t>("user");

    if(!userId) {
        const auto res = HttpResponse::newRedirectionResponse("/");
        cb(res);
        return;
    }
}


}  // namespace Ctrlr

