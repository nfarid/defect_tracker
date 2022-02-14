
#include "./session.hpp"

#include <sodium.h>

#include <array>
#include <iostream>


namespace Ctrlr
{


void Session::newGet(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb) {
    const auto session = req->session();
    if(!session)
        std::abort();
    const auto userId = session->getOptional<int32_t>("user");

    // If the user has already logged in, there's no point of the login page.
    if(userId)
        return cb(HttpResponse::newRedirectionResponse("/") );

    cb(HttpResponse::newHttpViewResponse("login.csp", {}) );
}

void Session::create(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb) {
    const auto& postParams = req->parameters();
    HttpViewData data;

    // Check if the username and password has been entered
    const auto usernameIter = postParams.find("login_username");
    if(usernameIter == end(postParams) ) {
        data.insert("error", "Username has not been entered");
        return cb(HttpResponse::newRedirectionResponse("/login") );
    }
    const auto& username = usernameIter->second;
    const auto passwordIter = postParams.find("login_password");
    if(passwordIter == end(postParams) ) {
        data.insert("error", "Password has not been entered");
        return cb(HttpResponse::newRedirectionResponse("/login") );
    }
    const auto& password = passwordIter->second;

    std::array<char, crypto_pwhash_STRBYTES> passwordHash{};
    if(crypto_pwhash_str(passwordHash.data(), password.c_str(), size(password),
                crypto_pwhash_OPSLIMIT_MODERATE, crypto_pwhash_MEMLIMIT_MODERATE) )
    {
        // TODO: Better error handling
        std::abort();
    }

    const Criteria criteria{Model::Account::Cols::_username, CompareOperator::EQ, username};
    try {
        const Model::Account user = mAccountOrm.findOne(criteria);
        req->session()->insert("user", user.getValueOfId() );
    } catch(std::exception& ex) {
        data.insert("error", "Invalid username or password");
        return cb(HttpResponse::newRedirectionResponse("/login") );
    }

    return cb(HttpResponse::newRedirectionResponse("/") );
}


}  // namespace Ctrlr

