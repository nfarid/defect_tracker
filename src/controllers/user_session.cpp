
#include "./auxiliary.hpp"
#include "../models/account.hpp"
#include "../models/auxiliary.hpp"
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
    void create(const HttpRequestPtr& req, ResponseCallback&& cb);
    static void destroy(const HttpRequestPtr& req, ResponseCallback&& cb);

private:
    Mapper<Model::Account> mAccountOrm = Mapper<Model::Account>(app().getDbClient("db") );
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

void UserSession::create(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb) {
    const auto& postParams = req->parameters();

    // The data for the signup view
    // If an error occurs then the user will have to retry the login
    HttpViewData data = getViewData("Login", *getSession(req) );
    data.insert("form_action", "/login"s);

    // Obtain the username & password
    std::string username;
    std::string password;
    try {
        username = postParams.at("form-username");
        password = postParams.at("form-password");
    }  catch(const std::exception& ex) {
        std::cerr<<ex.what()<<std::endl;
        data.insert("form_error", "Some of the required input has not been entered");
        auto resp = HttpResponse::newHttpViewResponse("user_form.csp", data);
        resp->setStatusCode(k401Unauthorized);
        return cb(resp);
    }

    // Check if the username & password is correct
    Model::Account user;
    try {
        user = findByUsername(username, mAccountOrm);
    }  catch(const std::exception& ex) {
        data.insert("form_error", "Incorrect username"s);
        auto resp = HttpResponse::newHttpViewResponse("user_form.csp", data);
        resp->setStatusCode(k401Unauthorized);
        return cb(resp);
    }
    if(!Util::verifyHash(user.getValueOfPasswordHash(), password) ) {
        // Incorrect password
        data.insert("form_error", "Incorrect password"s);
        data.insert("form_username", username);
        auto resp = HttpResponse::newHttpViewResponse("user_form.csp", data);
        resp->setStatusCode(k401Unauthorized);
        return cb(resp);
    }

    // Everything is correct, so the user can login
    logIn(*getSession(req), user.getValueOfId(), username);
    return cb(HttpResponse::newRedirectionResponse("/", k303SeeOther) );
}

void UserSession::destroy(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb)
{
    const SessionPtr session = getSession(req);
    // There's no check if the user is logged in or not, since the result is the same either way
    req->session()->clear();
    return cb(HttpResponse::newRedirectionResponse("/", k303SeeOther) );
}


}  // namespace Ctrlr

