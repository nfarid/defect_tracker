
#include "auxiliary.hpp"

#include "../models/account.hpp"
#include "../models/notification.hpp"
#include "../util/form_error.hpp"
#include "../util/misc.hpp"

#include <drogon/HttpController.h>

#include <iostream>
#include <string>


namespace Ctrlr
{


using namespace Aux;
using namespace drogon;
using namespace drogon::orm;

class HomeController : public HttpController<HomeController> {
public:
    /*NO-FORMAT*/
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(HomeController::about, "/about", Get);
        ADD_METHOD_TO(HomeController::index, "/", Get);
        ADD_METHOD_TO(HomeController::index, "/index", Get);
        ADD_METHOD_TO(HomeController::index, "/home", Get);
        ADD_METHOD_TO(HomeController::demoLogin, "/demo-login", Post);
    METHOD_LIST_END
    /*YES-FORMAT*/

    Task<HttpResponsePtr> about(HttpRequestPtr req);
    Task<HttpResponsePtr> index(HttpRequestPtr req);
    Task<HttpResponsePtr> demoLogin(HttpRequestPtr req);

private:
    CoroMapper<Model::Account> mAccountOrm = app().getDbClient("db");

    const std::vector<std::string_view> demoUsernameLst = {"demo_regular_user",
                                                           "demo_project_staff", "demo_project_manager"};
};

Task<HttpResponsePtr> HomeController::about(HttpRequestPtr req) {
    HttpViewData data = getViewData("Home", *getSession(req) );
    co_return HttpResponse::newHttpViewResponse("home_about.csp", data);
}

Task<HttpResponsePtr> HomeController::index(HttpRequestPtr req) {
    const SessionPtr session = getSession(req);

    HttpViewData data = getViewData("Home", *session);
    data.insert("demo-username-lst", Util::toJson(demoUsernameLst) );

    if(isLoggedIn(*session) ) {
        try {
            const Model::Account currentUser = co_await mAccountOrm.findByPrimaryKey( getUserId(*session) );
            const std::vector notifcationLst = co_await currentUser.getNotifications();
            data.insert("notification-lst", toViewJson(notifcationLst) );
        }  catch(const std::exception& ex) {
            std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<ex.what()<<std::endl;
            co_return HttpResponse::newNotFoundResponse();
        }
    } else {
        data.insert("notification-lst", Json::Value{});
    }

    co_return HttpResponse::newHttpViewResponse("home_index.csp", data);
}

Task<HttpResponsePtr> HomeController::demoLogin(HttpRequestPtr req) {
    const Util::StringMap& postParams = req->parameters();
    const SessionPtr session = getSession(req);

    try {
        CHECK_TOKEN();

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

