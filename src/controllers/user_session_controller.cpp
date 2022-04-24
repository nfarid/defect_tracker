
#include "auxiliary.hpp"
#include "../models/account.hpp"

#include "../util/core.hpp"
#include "../util/database.hpp"
#include "../util/form_error.hpp"
#include "../util/misc.hpp"

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
        ADD_METHOD_TO(UserSessionController::create, "/login", Post, "Fltr::ValidToken");
        ADD_METHOD_TO(UserSessionController::destroy, "/logout", Post, "Fltr::ValidToken");
        ADD_METHOD_TO(UserSessionController::demoLogin, "/demo-login", Post, "Fltr::ValidToken");
    METHOD_LIST_END
    /*YES-FORMAT*/

    Task<HttpResponsePtr> newForm(HttpRequestPtr req);
    Task<HttpResponsePtr> create(HttpRequestPtr req);
    Task<HttpResponsePtr> destroy(HttpRequestPtr req);
    // demoLogin is called whenever
    Task<HttpResponsePtr> demoLogin(HttpRequestPtr req);

private:
    CoroMapper<Model::Account> mAccountOrm = Util::getDb();

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
    const Util::StringMap& postParams = req->parameters();
    const SessionPtr session = getSession(req);

    try {
        const auto user = co_await Model::Account::verifyLogin(postParams);
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

Task<HttpResponsePtr> UserSessionController::destroy(HttpRequestPtr req) {
    SessionPtr session = getSession(req);
    logOut(*session);
    co_return HttpResponse::newRedirectionResponse("/", k303SeeOther);
}

Task<HttpResponsePtr> UserSessionController::demoLogin(HttpRequestPtr req) {
    const Util::StringMap& postParams = req->parameters();
    const SessionPtr session = getSession(req);

    try {
        const std::string& username = postParams.at("demo-username");
        if(!Util::contains(demoUsernameLst, username) )
            throw Util::FormError("Not a valid demo user");

        const Criteria hasUsername{Model::Account::Cols::_username, CompareOperator::EQ, username};
        const Model::Account user = co_await mAccountOrm.findOne(hasUsername);

        logIn(*session, user.getValueOfId(), username);
        co_return HttpResponse::newRedirectionResponse("/");
    }  catch(const std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}


}  // namespace Ctrlr

