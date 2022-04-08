
#include "auxiliary.hpp"
#include "../models/account.hpp"
#include "../util/core.hpp"
#include "../util/form_error.hpp"
#include "../util/hash.hpp"

#include <drogon/HttpController.h>

#include <array>
#include <iostream>
#include <string>


namespace Ctrlr
{


using std::string_literals::operator""s;
using namespace Aux;
using namespace drogon;
using namespace drogon::orm;

class UserSessionController : public drogon::HttpController<UserSessionController> {
public:
    /*NO-FORMAT*/
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(UserSessionController::newForm, "/login", Get);
        ADD_METHOD_TO(UserSessionController::create, "/login", Post);
        ADD_METHOD_TO(UserSessionController::destroy, "/logout", Post);
    METHOD_LIST_END
    /*YES-FORMAT*/

    Task<HttpResponsePtr> newForm(HttpRequestPtr req);
    Task<HttpResponsePtr> create(HttpRequestPtr req);
    static void destroy(const HttpRequestPtr& req, ResponseCallback&& cb);

private:
    CoroMapper<Model::Account> mAccountOrm{app().getDbClient("db")};

    HttpResponsePtr newImpl(HttpRequestPtr req, Util::StringMap formData,
            std::string errorMessage);
};


Task<HttpResponsePtr> UserSessionController::newForm(HttpRequestPtr req) {
    co_return newImpl(req, {}, "");
}

HttpResponsePtr UserSessionController::newImpl(HttpRequestPtr req,
        Util::StringMap formData, std::string errorMessage)
{
    const SessionPtr session = getSession(req);
    // If the user has already logged in, there's no point of the login page.
    if(isLoggedIn(*session) )
        return HttpResponse::newRedirectionResponse("/");

    HttpViewData data = getViewData("Login", *session);
    data.insert("form-action", "/login"s);

    // If there was an error, then redisplay the form data
    data.insert("form-error", errorMessage);
    for(const auto& [k, v] : formData)
        data.insert(k, HttpViewData::htmlTranslate(v) );

    return HttpResponse::newHttpViewResponse("user_form.csp", data);
}

Task<HttpResponsePtr> UserSessionController::create(HttpRequestPtr req) {
    const auto& postParams = req->parameters();

    try {
        const auto user = co_await Model::Account::verifyLogin(mAccountOrm, postParams);
        logIn(*getSession(req), user.getValueOfId(), user.getValueOfUsername() );
        co_return HttpResponse::newRedirectionResponse("/", k303SeeOther);
    } catch(const Util::FormError& ex) {
        // There was a form error, so let the user retry again
        co_return newImpl(req, postParams, ex.what() );
    }  catch(const std::exception& ex) {
        // An unexpected error has occured
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}

void UserSessionController::destroy(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb)
{
    const SessionPtr session = getSession(req);
    // There's no check if the user is logged in or not, since the result is the same either way
    req->session()->clear();
    return cb(HttpResponse::newRedirectionResponse("/", k303SeeOther) );
}


}  // namespace Ctrlr

