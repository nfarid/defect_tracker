
#include "./auxiliary.hpp"
#include "../models/account.hpp"
#include "../util/hash.hpp"

#include <drogon/HttpController.h>

#include <string>


namespace Ctrlr
{


using std::string_literals::operator""s;
using namespace Aux;
using namespace drogon;
using namespace drogon::orm;

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

    void show(const HttpRequestPtr&, ResponseCallback&& cb, int32_t id);
    static void newGet(const HttpRequestPtr& req, ResponseCallback&& cb);
    void create(const HttpRequestPtr& req, ResponseCallback&& cb);
    void destroy(const HttpRequestPtr& req, ResponseCallback&& cb, int32_t id);

private:
    Mapper<Model::Account> mAccountOrm = Mapper<Model::Account>(app().getDbClient("db") );
};


void User::show(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb, int32_t id)
{
    const SessionPtr session = getSession(req);

    try {
        const auto user = mAccountOrm.findByPrimaryKey(id);
        const std::string& username = user.getValueOfUsername();

        HttpViewData data = getViewData(username, *session);
        const auto currentUserId = session->getOptional<int32_t>("user");
        if(currentUserId && (*currentUserId == id) )
            data.insert("user_delete", "allowed"s);
        data.insert("user_username", username);
        data.insert("user_id", std::to_string(id) );

        return cb(HttpResponse::newHttpViewResponse("user.csp", data) );
    }  catch(std::exception& ex) {
        std::cerr<<ex.what()<<std::endl;
        return cb(HttpResponse::newNotFoundResponse() );
    }
}

void User::newGet(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb) {
    const SessionPtr session = getSession(req);
    // If the user has already logged in, there's no point of the signup page
    if(isLoggedIn(*session) )
        return cb(HttpResponse::newRedirectionResponse("/") );

    // Else show the signup page
    HttpViewData data = getViewData("Sign Up"s, *session);
    data.insert("form_action", "/signup"s);
    cb(HttpResponse::newHttpViewResponse("user_form.csp", data) );
}

// This seems to work, would need refactoring and testing
void User::create(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb) {
    // Data from the HTTP POST request
    const auto& postParams = req->parameters();

    // The data for the signup view
    HttpViewData data = getViewData("Sign Up"s, *getSession(req) );
    data.insert("form_action", "/signup"s);

    // Check if the username and password has been entered
    const auto usernameIter = postParams.find("form_username");
    if(usernameIter == end(postParams) ) {
        data.insert("form_error", "Username has not been entered"s);
        return cb(HttpResponse::newHttpViewResponse("user_form.csp", data) );
    }
    const auto& username = usernameIter->second;
    const auto passwordIter = postParams.find("form_password");
    if(passwordIter == end(postParams) ) {
        data.insert("form_error", "Password has not been entered"s);
        return cb(HttpResponse::newHttpViewResponse("user_form.csp", data) );
    }
    const auto passwordHash = Util::hash(passwordIter->second);

    // TODO: Add some requirements for the username and password here (e.g. no special characters)

    // Check that the username is not in the database
    const Criteria userCriteria{Model::Account::Cols::_username, CompareOperator::EQ, username};
    const auto userCount = mAccountOrm.count(userCriteria);
    if(userCount > 0) {
        data.insert("form_error", "That username already exist, try another username"s);
        return cb(HttpResponse::newHttpViewResponse("user_form.csp", data) );
    }

    // Create a new account (and then redirect to the home page)
    Model::Account newAccount;
    newAccount.setUsername(username);
    newAccount.setPasswordDigest(passwordHash);
    try {
        mAccountOrm.insert(newAccount);  // This will also set the id for newAccount
        SessionPtr session = getSession(req);
        session->insert("user_id", newAccount.getValueOfId() );
        session->insert("username", newAccount.getValueOfUsername() );
        return cb(HttpResponse::newRedirectionResponse("/", k303SeeOther) );
    }catch(std::exception& ex) {
        std::cerr<<ex.what()<<std::endl;
        data.insert("form_error", "There seems to be an error"s);
        return cb(HttpResponse::newHttpViewResponse("user_form.csp", data) );
    }
}

void User::destroy(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb, int32_t id)
{
    SessionPtr session = getSession(req);

    const auto userId = session->getOptional<int32_t>("user_id");
    if(!userId || (*userId != id) ) {
        // Should be http 403
        return cb(HttpResponse::newNotFoundResponse() );
    }

    session->clear();
    mAccountOrm.deleteByPrimaryKey(id);
    cb(HttpResponse::newRedirectionResponse("/", k303SeeOther) );
}


}  // namespace Ctrlr

