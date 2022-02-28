
#include "./auxiliary.hpp"
#include "../models/account.hpp"

#include <drogon/HttpController.h>

#include <iostream>
#include <string>


namespace Ctrlr
{


using namespace Aux;
using namespace drogon;
using namespace drogon::orm;

class Home : public HttpController<Home> {
public:
    /*NO-FORMAT*/
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(Home::index, "/", Get);
        ADD_METHOD_TO(Home::index, "/index", Get);
        ADD_METHOD_TO(Home::index, "/home", Get);
    METHOD_LIST_END
    /*YES-FORMAT*/

    static void index(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb);

private:
};


void Home::index(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb) {
    const SessionPtr session = getSession(req);
    HttpViewData data = getViewData("Home", *session);
    const auto response = HttpResponse::newHttpViewResponse("home.csp", data);
    cb(response);
}


}  // namespace Ctrlr

