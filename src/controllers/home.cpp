
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

    void index(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb);

private:
    Mapper<Model::Account> mAccountOrm = Mapper<Model::Account>(app().getDbClient("db") );
};


using std::string_literals::operator""s;


void Home::index(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb) {
    const SessionPtr session = getSession(req);

    const auto userId = session->getOptional<int32_t>("user");
    std::optional<std::string> username{};
    if(userId)
        username = mAccountOrm.findByPrimaryKey(*userId).getValueOfUsername();

    HttpViewData data;
    data.insert("title", "Home"s);
    data.insert("username", username);
    const auto response = HttpResponse::newHttpViewResponse("home.csp", data);
    cb(response);
}


}  // namespace Ctrlr

