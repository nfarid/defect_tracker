
#ifndef HPP_CONTROLLERS_USER_SESSION
#define HPP_CONTROLLERS_USER_SESSION

#include "../models/account.hpp"

#include <drogon/HttpController.h>


namespace Ctrlr
{


using namespace drogon;

class UserSession : public drogon::HttpController<UserSession> {
public:
    /*NO-FORMAT*/
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(UserSession::newGet, "/login", Get);
        ADD_METHOD_TO(UserSession::create, "/login", Post);
        ADD_METHOD_TO(UserSession::destroy, "/logout", Post);
    METHOD_LIST_END
    /*YES-FORMAT*/

    static void newGet(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb);
    void create(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb);
    static void destroy(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb);

private:
    Mapper<Model::Account> mAccountOrm = Mapper<Model::Account>(app().getDbClient("db") );
};


} // namespace Ctrlr

#endif  // ifndef HPP_CONTROLLERS_USER_SESSION
