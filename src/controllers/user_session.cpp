
#include "../models/account.hpp"
#include "../util/core.hpp"
#include "../util/hash.hpp"

#include <drogon/HttpController.h>

#include <array>
#include <iostream>
#include <string>


namespace Ctrlr
{


using namespace drogon;
using std::string_literals::operator""s;

class UserSession : public drogon::HttpController<UserSession> {
public:
    /*NO-FORMAT*/
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(UserSession::newGet, "/login", Get);
        ADD_METHOD_TO(UserSession::create, "/login", Post);
        ADD_METHOD_TO(UserSession::destroy, "/logout", Post);
    METHOD_LIST_END
    /*YES-FORMAT*/

    static void newGet(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb);
    void create(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb);
    static void destroy(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb);

private:
    Mapper<Model::Account> mAccountOrm = Mapper<Model::Account>(app().getDbClient("db") );
};


void UserSession::newGet(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb) {
    const auto session = req->session();
    if(!session) {
        std::cerr<<"Session is not enabled"<<std::endl;
        throw std::runtime_error("Session is not enabled");
    }
    const auto userId = session->getOptional<int32_t>("user");

    // If the user has already logged in, there's no point of the login page.
    if(userId)
        return cb(HttpResponse::newRedirectionResponse("/") );

    HttpViewData data;
    data.insert("title", "Login"s);
    cb(HttpResponse::newHttpViewResponse("login.csp", {}) );
}

void UserSession::create(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb) {
    const auto& postParams = req->parameters();
    HttpViewData data;
    data.insert("title", "Login"s);

    // Check if the username and password has been entered
    const auto usernameIter = postParams.find("login_username");
    if(usernameIter == end(postParams) ) {
        data.insert("login_error", "Username has not been entered"s);
        return cb(HttpResponse::newHttpViewResponse("login.csp", data) );
    }
    const auto& username = usernameIter->second;
    const auto passwordIter = postParams.find("login_password");
    if(passwordIter == end(postParams) ) {
        data.insert("login_error", "Password has not been entered"s);
        return cb(HttpResponse::newHttpViewResponse("login.csp", data) );
    }
    const auto& password = passwordIter->second;

    // Check if the username & password is correct

    const Criteria userCriteria{Model::Account::Cols::_username, CompareOperator::EQ, username};
    auto userFuture = mAccountOrm.findFutureBy(userCriteria);

    const auto passwordHash = Util::hash(password);

    try {
        const auto& foundUserLst = userFuture.get();
        if(foundUserLst.empty() ) {
            // No user with that name is in the datbase i.e. incorrect username
            data.insert("login_error", "Incorrect username"s);
            return cb(HttpResponse::newHttpViewResponse("login.csp", data) );
        }
        const auto& user = foundUserLst.front();

        if(!Util::verifyHash(user.getValueOfPasswordDigest(), password) ) {
            // Incorrect password
            data.insert("login_error", "Incorrect password"s);
            data.insert("login_username", username);
            return cb(HttpResponse::newHttpViewResponse("login.csp", data) );
        }

        // Everything is correct, so the user can login
        req->session()->insert("user", user.getValueOfId() );
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
    const auto session = req->session();
    if(!session) {
        std::cerr<<"Session is not enabled"<<std::endl;
        throw std::runtime_error("Session is not enabled");
    }
    // There's no check if the user is logged in or not, since the result is the same either way
    req->session()->clear();
    return cb(HttpResponse::newRedirectionResponse("/", k303SeeOther) );
}


}  // namespace Ctrlr

