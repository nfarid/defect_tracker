
#include "./user.hpp"
#include "../util/hash.hpp"

#include <string>


namespace Ctrlr
{


using std::string_literals::operator""s;


void User::newGet(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb) {
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

// This seems to work, would need refactoring and testing
void User::create(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb) {
    // TODO: Refactor this
    const auto& postParams = req->parameters();
    HttpViewData data;
    data.insert("title", "Signup"s);

    // Check if the username and password has been entered
    const auto usernameIter = postParams.find("signup_username");
    if(usernameIter == end(postParams) ) {
        data.insert("signup_error", "Username has not been entered"s);
        return cb(HttpResponse::newHttpViewResponse("signup.csp", data) );
    }
    const auto& username = usernameIter->second;
    const auto passwordIter = postParams.find("signup_password");
    if(passwordIter == end(postParams) ) {
        data.insert("signup_error", "Password has not been entered"s);
        return cb(HttpResponse::newHttpViewResponse("signup.csp", data) );
    }
    const auto passwordHash = Util::hash(passwordIter->second);

    // TODO: Add some requirements for the username and password here (e.g. no special characters)

    // Check that the username is not in the database
    const Criteria userCriteria{Model::Account::Cols::_username, CompareOperator::EQ, username};
    const auto userCount = mAccountOrm.count(userCriteria);
    if(userCount > 0) {
        data.insert("signup_error", "That username already exist, try another username"s);
        return cb(HttpResponse::newHttpViewResponse("signup.csp", data) );
    }

    // Create a new account
    Model::Account newAccount;
    newAccount.setUsername(username);
    newAccount.setPasswordDigest(passwordHash);
    try {
        mAccountOrm.insert(newAccount);
        std::cerr<<newAccount.getValueOfId()<<std::endl;
        req->session()->insert("user", newAccount.getValueOfId() );
        return cb(HttpResponse::newRedirectionResponse("/", k201Created) );
    }catch(std::exception& ex) {
        std::cerr<<ex.what()<<std::endl;
        data.insert("signup_error", "There seems to be an error"s);
        return cb(HttpResponse::newHttpViewResponse("signup.csp", data) );
    }
}


}  // namespace Ctrlr

