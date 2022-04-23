
#include "auxiliary.hpp"
#include "../models/account.hpp"

#include "../util/database.hpp"
#include "../util/form_error.hpp"

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
        ADD_METHOD_TO(UserController::show, "/user/{1}", Get);
        ADD_METHOD_TO(UserController::newForm, "/signup", Get);
        ADD_METHOD_TO(UserController::create, "/signup", Post, "Fltr::ValidToken");
    METHOD_LIST_END
    /*YES-FORMAT*/

    Task<HttpResponsePtr> show(HttpRequestPtr req, IdType id);
    Task<HttpResponsePtr> newForm(HttpRequestPtr req);
    Task<HttpResponsePtr> create(HttpRequestPtr req);

private:
    CoroMapper<Model::Account> mAccountOrm = Util::getDb();

    HttpResponsePtr newImpl(HttpRequestPtr req, Util::StringMap formData,
            std::string errorMessage);
};


Task<HttpResponsePtr> UserController::show(HttpRequestPtr req, IdType id)
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

Task<HttpResponsePtr> UserController::newForm(HttpRequestPtr req) {
    co_return newImpl(req, {}, {});
}

HttpResponsePtr UserController::newImpl(HttpRequestPtr req,
        Util::StringMap formData, std::string errorMessage)
{
    const SessionPtr session = getSession(req);
    // If the user has already logged in, there's no point of the signup page
    if(isLoggedIn(*session) )
        return HttpResponse::newRedirectionResponse("/");

    // Else show the signup page
    HttpViewData data = getViewData("Sign Up"s, *session);
    data.insert("form-action", "/signup"s);

    // If there was an error, then redisplay the form data
    data.insert("form-error", errorMessage);
    for(const auto& [k, v] : formData)
        data.insert(k, HttpViewData::htmlTranslate(v) );

    return HttpResponse::newHttpViewResponse("user_form.csp", data);
}

Task<HttpResponsePtr> UserController::create(HttpRequestPtr req) {
    // Data from the HTTP POST request
    const Util::StringMap& postParams = req->parameters();
    const SessionPtr session = getSession(req);

    try {
        const Model::Account account = co_await Model::Account::createAccount(postParams);
        // If the form data is valid and the user can be created, then login
        logIn(*getSession(req), account.getValueOfId(), account.getValueOfUsername() );
        co_return HttpResponse::newRedirectionResponse("/", k303SeeOther);
    } catch(Util::FormError& ex) {
        co_return newImpl(req, postParams, ex.what() );  // There was a form error, so let the user retry again
    }  catch(const std::exception& ex) {
        // An unexpected error has occured
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}


}  // namespace Ctrlr

