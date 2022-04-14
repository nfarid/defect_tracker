
#include "auxiliary.hpp"
#include "../models/account.hpp"

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
        ADD_METHOD_TO(HomeController::index, "/", Get);
        ADD_METHOD_TO(HomeController::index, "/index", Get);
        ADD_METHOD_TO(HomeController::index, "/home", Get);
    METHOD_LIST_END
    /*YES-FORMAT*/

    Task<HttpResponsePtr> index(HttpRequestPtr req);

private:
};


Task<HttpResponsePtr> HomeController::index(HttpRequestPtr req) {
    const SessionPtr session = getSession(req);
    HttpViewData data = getViewData("Home", *session);
    co_return HttpResponse::newHttpViewResponse("home.csp", data);
}


}  // namespace Ctrlr

