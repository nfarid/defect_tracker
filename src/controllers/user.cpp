
#include "../util/hash.hpp"
#include "../models/account.hpp"

#include <drogon/HttpController.h>

#include <string>


namespace Ctrlr
{


using namespace drogon;
using std::string_literals::operator""s;

class User : public drogon::HttpController<User> {
public:
    /*NO-FORMAT*/
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(User::show, "/user/{1}", Get);
        ADD_METHOD_TO(User::newGet, "/signup", Get);
        ADD_METHOD_TO(User::create, "/signup", Post);
        ADD_METHOD_TO(User::destroy, "/user/{1}/delete", Post);
    METHOD_LIST_END
    /*YES-FORMAT*/

    void show(const HttpRequestPtr&, std::function<void(const HttpResponsePtr&)>&& cb, int32_t id);
    static void newGet(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb);
    void create(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb);
    void destroy(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb, int32_t id);

private:
    Mapper<Model::Account> mAccountOrm = Mapper<Model::Account>(app().getDbClient("db") );
};


void User::show(const HttpRequestPtr&, std::function<void(const HttpResponsePtr&)>&& cb, int32_t id)
{
    HttpViewData data;
    try {
        const auto user = mAccountOrm.findByPrimaryKey(id);
        const auto& username = user.getValueOfUsername();
        data.insert("user_username", username);
        return cb(HttpResponse::newHttpViewResponse("user.csp", data) );
    }  catch(std::exception& ex) {
        std::cerr<<ex.what()<<std::endl;
        return cb(HttpResponse::newNotFoundResponse() );
    }
}

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
        mAccountOrm.insert(newAccount);  // This will also set the id for newAccount
        req->session()->insert("user", newAccount.getValueOfId() );
        return cb(HttpResponse::newRedirectionResponse("/", k303SeeOther) );
    }catch(std::exception& ex) {
        std::cerr<<ex.what()<<std::endl;
        data.insert("signup_error", "There seems to be an error"s);
        return cb(HttpResponse::newHttpViewResponse("signup.csp", data) );
    }
}

void User::destroy(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb, int32_t id)
{
    const auto session = req->session();
    if(!session) {
        std::cerr<<"Session is not enabled"<<std::endl;
        throw std::runtime_error("Session is not enabled");
    }

    const auto userId = session->getOptional<int32_t>("user");
    if(!userId || (*userId != id) )
        return cb(HttpResponse::newNotFoundResponse() );

    mAccountOrm.deleteByPrimaryKey(id);
    cb(HttpResponse::newRedirectionResponse("/", k303SeeOther) );
}


}  // namespace Ctrlr

