
#include "./user.hpp"

#include <string>


namespace Ctrlr
{


using std::string_literals::operator""s;


void User::newGet(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb) {
    std::cerr<<"Test!"<<std::endl;
    // TODO: Refactor this
    const auto session = req->session();
    if(!session) {
        std::cerr<<"Session is not enabled"<<std::endl;
        throw std::runtime_error("Session is not enabled");
    }
    const auto userId = session->getOptional<int32_t>("user");

    // If the user has already logged in, there's no point of the signup page
    if(userId)
        return cb(HttpResponse::newRedirectionResponse("/") );

    // Else show the signup page
    HttpViewData data;
    data.insert("title", "Signup"s);
    cb(HttpResponse::newHttpViewResponse("signup.csp", {}) );
}

void User::create(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb) {
    //
}


}  // namespace Ctrlr

