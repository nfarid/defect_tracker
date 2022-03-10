
#include "./auxiliary.hpp"
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
    HttpViewData data = getViewData("Login", *getSession(req) );

    // Check if the username and password has been entered
    const auto usernameIter = postParams.find("form_username");
    if(usernameIter == end(postParams) ) {
        data.insert("form_error", "Username has not been entered"s);
        auto resp = HttpResponse::newHttpViewResponse("user_form.csp", data);
        resp->setStatusCode(k401Unauthorized);
        return cb(resp);
    }
    const std::string& username = usernameIter->second;
    const auto passwordIter = postParams.find("form_password");
    if(passwordIter == end(postParams) ) {
        data.insert("form_username", username);
        data.insert("form_error", "Password has not been entered"s);
        auto resp = HttpResponse::newHttpViewResponse("user_form.csp", data);
        resp->setStatusCode(k401Unauthorized);
        return cb(resp);
    }
    const std::string& password = passwordIter->second;

    // Check if the username & password is correct

    const Criteria userCriteria{Model::Account::Cols::_username, CompareOperator::EQ, username};
    auto userFuture = mAccountOrm.findFutureBy(userCriteria);

    const auto passwordHash = Util::hash(password);

    try {
        const auto& foundUserLst = userFuture.get();
        if(foundUserLst.empty() ) {
            // No user with that name is in the datbase i.e. incorrect username
            data.insert("form_error", "Incorrect username"s);
            auto resp = HttpResponse::newHttpViewResponse("user_form.csp", data);
            resp->setStatusCode(k401Unauthorized);
            return cb(resp);
        }
        const auto& user = foundUserLst.front();

        if(!Util::verifyHash(user.getValueOfPasswordHash(), password) ) {
            // Incorrect password
            data.insert("form_error", "Incorrect password"s);
            data.insert("form_username", username);
            auto resp = HttpResponse::newHttpViewResponse("user_form.csp", data);
            resp->setStatusCode(k401Unauthorized);
            return cb(resp);
        }

        // Everything is correct, so the user can login
        SessionPtr session = getSession(req);
        session->insert("user_id", user.getValueOfId() );
        session->insert("username", user.getValueOfUsername() );
        return cb(HttpResponse::newRedirectionResponse("/", k303SeeOther) );
    } catch(std::exception& ex) {
        // TODO: Better error handling
        std::cerr<<ex.what()<<std::endl;
        std::abort();
    }
    UTIL_UNREACHABLE();
}

void UserSession::destroy(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb)
{
    const SessionPtr session = getSession(req);
    // There's no check if the user is logged in or not, since the result is the same either way
    req->session()->clear();
    return cb(HttpResponse::newRedirectionResponse("/", k303SeeOther) );
}


}  // namespace Ctrlr

