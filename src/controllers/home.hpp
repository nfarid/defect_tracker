
#ifndef HPP_HOME_CONTROLLERS
#define HPP_HOME_CONTROLLERS

#include "../models/account.hpp"

#include <drogon/HttpController.h>


namespace Ctrlr
{


using namespace drogon;
using namespace drogon::orm;

class Home : public drogon::HttpController<Home> {
public:
    /*NO-FORMAT*/
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(Home::get, "/", drogon::Get);
        ADD_METHOD_TO(Home::get, "/index", drogon::Get);
    METHOD_LIST_END
    /*YES-FORMAT*/

    void get(const drogon::HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb);

private:
    Mapper<Model::Account> m_account_orm = Mapper<Model::Account>(app().getDbClient("db") );
};


}// namespace Ctrl

#endif  // ifndef HPP_HOME_CONTROLLERS
