
#include "auxiliary.hpp"

#include "../models/account.hpp"
#include "../models/notification.hpp"

#include "../util/database.hpp"
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
using Model::Account;

class HomeController : public HttpController<HomeController> {
public:
    /*NO-FORMAT*/
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(HomeController::aboutPage, "/about", Get);
        ADD_METHOD_TO(HomeController::indexPage, "/", Get);
        ADD_METHOD_TO(HomeController::indexPage, "/index", Get);
        ADD_METHOD_TO(HomeController::indexPage, "/home", Get);
    METHOD_LIST_END
    /*YES-FORMAT*/

    // aboutPage is called whenever the user accesses the abouts page
    Task<HttpResponsePtr> aboutPage(HttpRequestPtr req);

    // indexPage is called whenever the user access the index page
    Task<HttpResponsePtr> indexPage(HttpRequestPtr req);

private:
    CoroMapper<Model::Account> mAccountOrm = Util::getDb();
};

Task<HttpResponsePtr> HomeController::aboutPage(HttpRequestPtr req) {
    HttpViewData data = getViewData("Home", *getSession(req) );
    co_return HttpResponse::newHttpViewResponse("home_about.csp", data);
}

Task<HttpResponsePtr> HomeController::indexPage(HttpRequestPtr req) {
    const SessionPtr session = getSession(req);

    HttpViewData viewData = getViewData("Home", *session);
    viewData.insert("demo-username-lst", Util::toJson(demoUsernameLst) );

    // If the user is logged in then add the user's notifications to the view
    if(isLoggedIn(*session) ) {
        const IdType currentUserId = getUserId(*session);
        try {
            const Account currentUser = co_await Account::findByPrimaryKey(currentUserId);
            const std::vector notifcationLst = co_await currentUser.getNotifications();
            viewData.insert("notification-lst", toViewJson(notifcationLst) );
        }  catch(const std::exception& ex) {
            std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<ex.what()<<std::endl;
            co_return HttpResponse::newNotFoundResponse();
        }
    } else {
        viewData.insert("notification-lst", Json::Value{});
    }

    co_return HttpResponse::newHttpViewResponse("home_index.csp", viewData);
}


}  // namespace Ctrlr

