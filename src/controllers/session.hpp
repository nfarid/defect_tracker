
#ifndef HPP_CONTROLLERS_SESSION
#define HPP_CONTROLLERS_SESSION

#include "../models/account.hpp"


#include <drogon/HttpController.h>


namespace Ctrlr
{


using namespace drogon;

class Session : public drogon::HttpController<Session> {
public:
    /*NO-FORMAT*/
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(Session::newGet, "/login", Get);
        //ADD_METHOD_TO(Home::create, "/login", Post);
    METHOD_LIST_END
    /*YES-FORMAT*/

    void newGet(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb);

private:
    Mapper<Model::Account> mAccountOrm = Mapper<Model::Account>(app().getDbClient("db") );
};


} // namespace Ctrlr

#endif  // ifndef HPP_CONTROLLERS_SESSION
