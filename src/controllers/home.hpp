
#ifndef HPP_CONTROLLERS_HOME
#define HPP_CONTROLLERS_HOME

#include "../models/account.hpp"


#include <drogon/HttpController.h>


namespace Ctrlr
{


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


}// namespace Ctrlr

#endif  // ifndef HPP_CONTROLLERS_HOME
