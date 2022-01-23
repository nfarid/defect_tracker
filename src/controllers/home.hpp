
#ifndef HPP_HOME_CONTROLLERS
#define HPP_HOME_CONTROLLERS

#include <drogon/HttpController.h>


namespace Ctrlr
{


using namespace drogon;

class Home : public drogon::HttpController<Home> {
public:
    /*NO-FORMAT*/
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(Home::get, "/", drogon::Get);
        ADD_METHOD_TO(Home::get, "/index", drogon::Get);
    METHOD_LIST_END
    /*YES-FORMAT*/

    void get(const drogon::HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb);
};


}// namespace Ctrl

#endif  // ifndef HPP_HOME_CONTROLLERS
