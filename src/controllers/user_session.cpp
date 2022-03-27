
#include "auxiliary.hpp"
#include "../models/account.hpp"
#include "../util/core.hpp"
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

class UserSession : public drogon::HttpController<UserSession> {
public:
    /*NO-FORMAT*/
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(UserSession::newForm, "/login", Get);
        ADD_METHOD_TO(UserSession::create, "/login", Post);
        ADD_METHOD_TO(UserSession::destroy, "/logout", Post);
    METHOD_LIST_END
    /*YES-FORMAT*/

    static void newForm(const HttpRequestPtr& req, ResponseCallback&& cb);
    Task<HttpResponsePtr> create(HttpRequestPtr req);
    static void destroy(const HttpRequestPtr& req, ResponseCallback&& cb);

private:
    CoroMapper<Model::Account> mAccountOrm{app().getDbClient("db")};
};


void UserSession::newForm(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb) {
    const SessionPtr session = getSession(req);
    // If the user has already logged in, there's no point of the login page.
    if(isLoggedIn(*session) )
        return cb(HttpResponse::newRedirectionResponse("/") );

    HttpViewData data = getViewData("Login", *session);
    data.insert("form_action", "/login"s);
    cb(HttpResponse::newHttpViewResponse("user_form.csp", data) );
}

Task<HttpResponsePtr> UserSession::create(HttpRequestPtr req) {
    const auto& postParams = req->parameters();

    try {
        const auto user = co_await Model::Account::verifyLogin(mAccountOrm, postParams);
        // Everything is correct, so the user can login
        logIn(*getSession(req), user.getValueOfId(), user.getValueOfUsername() );
        co_return HttpResponse::newRedirectionResponse("/", k303SeeOther);
    }  catch(const std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<ex.what()<<std::endl;
        HttpViewData data = getViewData("Login", *getSession(req) );
        data.insert("form_action", "/login"s);
        data.insert("form_error", "There seems to be an error. Try again."s);
        co_return HttpResponse::newHttpViewResponse("user_form.csp", data);
    }
}

void UserSession::destroy(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb)
{
    const SessionPtr session = getSession(req);
    // There's no check if the user is logged in or not, since the result is the same either way
    req->session()->clear();
    return cb(HttpResponse::newRedirectionResponse("/", k303SeeOther) );
}


}  // namespace Ctrlr

