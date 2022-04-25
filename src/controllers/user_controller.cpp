
#include "auxiliary.hpp"
#include "../models/account.hpp"

#include "../util/database.hpp"
#include "../util/form_error.hpp"
#include "../util/misc.hpp"

#include <drogon/HttpController.h>

#include <string>


namespace Ctrlr
{


using std::string_literals::operator""s;
using namespace Aux;
using namespace drogon;
using namespace drogon::orm;

class UserController : public drogon::HttpController<UserController> {
public:
    /*NO-FORMAT*/
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(UserController::showUser, "/user/{1}", Get);
        ADD_METHOD_TO(UserController::signupForm, "/signup", Get);
        ADD_METHOD_TO(UserController::signupSubmit, "/signup", Post, "Fltr::ValidToken");
        ADD_METHOD_TO(UserController::loginForm, "/login", Get);
        ADD_METHOD_TO(UserController::loginSubmit, "/login", Post, "Fltr::ValidToken");
        ADD_METHOD_TO(UserController::logout, "/logout", Post, "Fltr::ValidToken");
        ADD_METHOD_TO(UserController::demoLogin, "/demo-login", Post, "Fltr::ValidToken");
    METHOD_LIST_END
    /*YES-FORMAT*/

    Task<HttpResponsePtr> showUser(HttpRequestPtr req, IdType userId);

    Task<HttpResponsePtr> signupForm(HttpRequestPtr req);
    Task<HttpResponsePtr> signupSubmit(HttpRequestPtr req);

    Task<HttpResponsePtr> loginForm(HttpRequestPtr req);
    Task<HttpResponsePtr> loginSubmit(HttpRequestPtr req);
    Task<HttpResponsePtr> logout(HttpRequestPtr req);
    Task<HttpResponsePtr> demoLogin(HttpRequestPtr req);

private:
    CoroMapper<Model::Account> mAccountOrm = Util::getDb();

    HttpResponsePtr signupFormImpl(HttpRequestPtr req, const Util::StringMap& formData, const std::string& formError);
    HttpResponsePtr loginFormImpl(HttpRequestPtr req, const Util::StringMap& formData, const std::string& formError);
};


Task<HttpResponsePtr> UserController::showUser(HttpRequestPtr req, IdType id)
{
    const SessionPtr session = getSession(req);
    try {
        const Model::Account user = co_await mAccountOrm.findByPrimaryKey(id);
        // Add the user's info to the ViewData to be sent to the view
        HttpViewData data = getViewData(user.getValueOfUsername(), *session);
        data.insert("user", user.toViewJson() );

        co_return HttpResponse::newHttpViewResponse("user_show.csp", data);
    }  catch(const std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}

Task<HttpResponsePtr> UserController::signupForm(HttpRequestPtr req) {
    co_return signupFormImpl(req, {}, {});
}

HttpResponsePtr UserController::signupFormImpl(HttpRequestPtr req, const Util::StringMap& formData,
        const std::string& formError)
{
    const SessionPtr session = getSession(req);

    if(isLoggedIn(*session) ) // If the user has already logged in, there's no point of the signup page
        return HttpResponse::newRedirectionResponse("/");

    // Else show the signup page
    HttpViewData data = getViewData("Sign Up"s, *session);
    data.insert("form-action", "/signup"s);

    // If there was an error, then redisplay the form data
    data.insert("form-error", formError);
    for(const auto& [k, v] : formData)
        data.insert(k, HttpViewData::htmlTranslate(v) );

    return HttpResponse::newHttpViewResponse("user_form.csp", data);
}

Task<HttpResponsePtr> UserController::signupSubmit(HttpRequestPtr req) {
    // Data from the HTTP POST request
    const Util::StringMap& postParams = req->parameters();
    const SessionPtr session = getSession(req);

    try {
        const Model::Account account = co_await Model::Account::createAccount(postParams);
        // If the form data is valid and the user can be created, then login
        logIn(*getSession(req), account.getValueOfId(), account.getValueOfUsername() );
        co_return HttpResponse::newRedirectionResponse("/", k303SeeOther);
    } catch(Util::FormError& ex) {
        co_return signupFormImpl(req, postParams, ex.what() );  // There was a form error, so let the user retry again
    }  catch(const std::exception& ex) {
        // An unexpected error has occured
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}

Task<HttpResponsePtr> UserController::loginForm(HttpRequestPtr req) {
    co_return loginFormImpl(req, {}, "");
}

HttpResponsePtr UserController::loginFormImpl(HttpRequestPtr req, const Util::StringMap& formData,
        const std::string& formError)
{
    const SessionPtr session = getSession(req);

    if(isLoggedIn(*session) ) // If the user has already logged in, there's no point of the login page.
        return HttpResponse::newRedirectionResponse("/");

    HttpViewData data = getViewData("Login", *session);
    data.insert("form-action", "/login"s);

    // If there was an error, then redisplay the form data
    data.insert("form-error", formError);
    for(const auto& [k, v] : formData)
        data.insert(k, HttpViewData::htmlTranslate(v) );

    return HttpResponse::newHttpViewResponse("user_form.csp", data);
}

Task<HttpResponsePtr> UserController::loginSubmit(HttpRequestPtr req) {
    const Util::StringMap& postParams = req->parameters();
    const SessionPtr session = getSession(req);

    try {
        const auto user = co_await Model::Account::verifyLogin(postParams);
        logIn(*getSession(req), user.getValueOfId(), user.getValueOfUsername() );
        co_return HttpResponse::newRedirectionResponse("/", k303SeeOther);
    } catch(const Util::FormError& ex) {
        // There was a form error, so let the user retry again
        co_return loginFormImpl(req, postParams, ex.what() );
    }  catch(const std::exception& ex) {
        // An unexpected error has occured
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}

Task<HttpResponsePtr> UserController::logout(HttpRequestPtr req) {
    SessionPtr session = getSession(req);
    logOut(*session);
    co_return HttpResponse::newRedirectionResponse("/", k303SeeOther);
}

Task<HttpResponsePtr> UserController::demoLogin(HttpRequestPtr req) {
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

