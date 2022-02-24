
#ifndef HPP_CONTROLLERS_USER
#define HPP_CONTROLLERS_USER

#include "../models/account.hpp"

#include <drogon/HttpController.h>


namespace Ctrlr
{


using namespace drogon;

class User : public drogon::HttpController<User> {
public:
    /*NO-FORMAT*/
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(User::newGet, "/signup", Get);
    ADD_METHOD_TO(User::create, "/signup", Post);
    METHOD_LIST_END
    /*YES-FORMAT*/

    static void newGet(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb);
    void create(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb);

private:
    Mapper<Model::Account> mAccountOrm = Mapper<Model::Account>(app().getDbClient("db") );
};


}// namespace Ctrlr

#endif  // ifndef HPP_CONTROLLERS_USER
