
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
using Model::Account;
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

    // showProject is called when accessing a user's page
    Task<HttpResponsePtr> showUser(HttpRequestPtr req, IdType userId);

    // signupForm is called when the user accesses the signup form
    Task<HttpResponsePtr> signupForm(HttpRequestPtr req);
    // signupSubmit is called when the user submits from the signup form
    Task<HttpResponsePtr> signupSubmit(HttpRequestPtr req);

    // signupForm is called when the user accesses the login form
    Task<HttpResponsePtr> loginForm(HttpRequestPtr req);
    // signupSubmit is called when the user submits from the login form
    Task<HttpResponsePtr> loginSubmit(HttpRequestPtr req);
    // logout is called when the user wants to logout
    Task<HttpResponsePtr> logout(HttpRequestPtr req);
    // demoLogin is when the user clicks on one of the demo login buttons
    Task<HttpResponsePtr> demoLogin(HttpRequestPtr req);

private:
    HttpResponsePtr formImpl(HttpRequestPtr req, const std::string& formAction,
            const Util::StringMap& previousFormData, const std::string& previousFormError);
};


Task<HttpResponsePtr> UserController::showUser(HttpRequestPtr req, IdType userId)
{
    const SessionPtr session = getSession(req);
    try {
        const Account user = co_await Account::findByPrimaryKey(userId);
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
    co_return formImpl(req, "signup", {}, {});
}

Task<HttpResponsePtr> UserController::signupSubmit(HttpRequestPtr req) {
    // Data from the HTTP POST request
    const Util::StringMap& formData = req->parameters();
    const SessionPtr session = getSession(req);

    try {
        const Account user = co_await Account::createAccount(formData);
        // If the form data is valid and the user can be created, then login
        logIn(*getSession(req), user.getValueOfId(), user.getValueOfUsername() );
        co_return HttpResponse::newRedirectionResponse("/");  // redirect to the home page
    } catch(Util::FormError& ex) {
        co_return formImpl(req, "signup", formData, ex.what() );  // There was a form error, so let the user retry again
    }  catch(const std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<typeid(ex).name()<<" ; "<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}

Task<HttpResponsePtr> UserController::loginForm(HttpRequestPtr req) {
    co_return formImpl(req, "login", {}, "");
}

Task<HttpResponsePtr> UserController::loginSubmit(HttpRequestPtr req) {
    const Util::StringMap& formData = req->parameters();
    const SessionPtr session = getSession(req);

    try {
        // Verify the submitted data, if authenticated then login
        const Account user = co_await Account::verifyLogin(formData);
        logIn(*getSession(req), user.getValueOfId(), user.getValueOfUsername() );
        co_return HttpResponse::newRedirectionResponse("/");  // redirect to the home page
    } catch(const Util::FormError& ex) {
        co_return formImpl(req, "login", formData, ex.what() );  // There was a form error, so let the user retry again
    }  catch(const std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<typeid(ex).name()<<" ; "<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}

Task<HttpResponsePtr> UserController::logout(HttpRequestPtr req) {
    SessionPtr session = getSession(req);
    logOut(*session);
    co_return HttpResponse::newRedirectionResponse("/");
}

Task<HttpResponsePtr> UserController::demoLogin(HttpRequestPtr req) {
    const SessionPtr session = getSession(req);

    try {
        const std::string& username = req->parameters().at("demo-username");
        if(!Util::contains(demoUsernameLst, username) )
            throw Util::FormError("Not a valid demo user");

        const Account user = co_await Account::findByUsername(username);
        Aux::logIn(*session, user.getValueOfId(), username);
        co_return HttpResponse::newRedirectionResponse("/");  // redirect to the home page
    }  catch(const std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<typeid(ex).name()<<" ; "<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}

HttpResponsePtr UserController::formImpl(HttpRequestPtr req, const std::string& formAction,
        const Util::StringMap& previousFormData, const std::string& previousFormError)
{
    const SessionPtr session = getSession(req);

    if(isLoggedIn(*session) ) // If the user has already logged in, there's no point of signup/login
        return HttpResponse::newRedirectionResponse("/");

    // Else show the signup page
    HttpViewData data = getViewData(formAction, *session);
    data.insert("form-action", "/" + formAction);

    // Add the form error message and previous form data if redisplaying the form
    data.insert("form-error", previousFormError);
    for(const auto& [k, v] : previousFormData)
        data.insert(k, HttpViewData::htmlTranslate(v) );

    // Display the user form
    return HttpResponse::newHttpViewResponse("user_form.csp", data);
}


}  // namespace Ctrlr

