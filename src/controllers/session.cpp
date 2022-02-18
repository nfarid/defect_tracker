
#include "./session.hpp"
#include "../util/core.hpp"
#include "../util/hash.hpp"

#include <array>
#include <iostream>
#include <string>


namespace Ctrlr
{


using std::string_literals::operator""s;


void Session::newGet(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb) {
    const auto session = req->session();
    if(!session)
        std::abort();
    const auto userId = session->getOptional<int32_t>("user");

    // If the user has already logged in, there's no point of the login page.
    if(userId)
        return cb(HttpResponse::newRedirectionResponse("/") );

    HttpViewData data;
    data.insert("title", "Login"s);
    cb(HttpResponse::newHttpViewResponse("login.csp", {}) );
}

void Session::create(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb) {
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
        return cb(HttpResponse::newRedirectionResponse("/") );
    } catch(std::exception& ex) {
        // TODO: Better error handling
        std::cerr<<ex.what()<<std::endl;
        std::abort();
    }
    UTIL_UNREACHABLE();
}


}  // namespace Ctrlr

