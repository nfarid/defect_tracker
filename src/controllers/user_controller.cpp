
#include "auxiliary.hpp"
#include "../models/account.hpp"
#include "../util/form_error.hpp"
#include "../util/hash.hpp"

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
        ADD_METHOD_TO(UserController::create, "/signup", Post);
        ADD_METHOD_TO(UserController::destroy, "/user/{1}/delete", Post);
    METHOD_LIST_END
    /*YES-FORMAT*/

    Task<HttpResponsePtr> show(HttpRequestPtr req, int32_t id);
    Task<HttpResponsePtr> newForm(HttpRequestPtr req);
    Task<HttpResponsePtr> create(HttpRequestPtr req);
    Task<HttpResponsePtr> destroy(HttpRequestPtr req, int32_t id);

private:
    CoroMapper<Model::Account> mAccountOrm{app().getDbClient("db")};

    HttpResponsePtr newImpl(HttpRequestPtr req, Util::StringMap formData,
            std::string errorMessage);
};


Task<HttpResponsePtr> UserController::show(HttpRequestPtr req, int32_t id)
{
    const SessionPtr session = getSession(req);
    try {
        const Model::Account user = co_await mAccountOrm.findByPrimaryKey(id);
        const std::string& username = user.getValueOfUsername();
        // Add the user's info to the ViewData to be sent to the view
        HttpViewData data = getViewData(username, *session);
        data.insert("user_username", username);
        data.insert("user_id", std::to_string(id) );
        const std::optional userIdOpt = session->getOptional<int32_t>("user");
        if(userIdOpt && (*userIdOpt == id) ) // Only the user can delete their own account
            data.insert("can_delete", true);

        co_return HttpResponse::newHttpViewResponse("user.csp", data);
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
        data.insert(k, v);

    return HttpResponse::newHttpViewResponse("user_form.csp", data);
}

Task<HttpResponsePtr> UserController::create(HttpRequestPtr req) {
    // Data from the HTTP POST request
    const auto& postParams = req->parameters();

    try {
        const Model::Account account = co_await Model::Account::createAccount(mAccountOrm, postParams);
        // If the form data is valid and the user can be created, then login
        logIn(*getSession(req), account.getValueOfId(), account.getValueOfUsername() );
        co_return HttpResponse::newRedirectionResponse("/", k303SeeOther);
    } catch(Util::FormError& ex) {
        // There was a form error, so let the user retry again
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<ex.what()<<std::endl;
        co_return newImpl(req, postParams, ex.what() );
    }  catch(const std::exception& ex) {
        // An unexpected error has occured
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}

Task<HttpResponsePtr> UserController::destroy(HttpRequestPtr req, int32_t id)
{
    SessionPtr session = getSession(req);
    const std::optional userIdOpt = session->getOptional<int32_t>("user_id");
    if(!userIdOpt || (*userIdOpt != id) ) // Only the user can delete their own account
        co_return HttpResponse::newNotFoundResponse();// Should be http 403

    session->clear();
    co_await mAccountOrm.deleteByPrimaryKey(id);
    co_return HttpResponse::newRedirectionResponse("/", k303SeeOther);
}


}  // namespace Ctrlr

