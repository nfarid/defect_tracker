
#ifndef HPP_CONTROLLERS_AUXILIARY
#define HPP_CONTROLLERS_AUXILIARY

// Some auxiliary functions for the controllers

#include <drogon/Session.h>
#include <drogon/HttpRequest.h>


namespace Aux
{


/**
 * \brief getSession Obtains the session from a http request
 * \throws std::runtime_error if session is not enabled
 */
drogon::SessionPtr getSession(const drogon::HttpRequestPtr& req);


}  // namespace Aux

#endif  // ifndef HPP_CONTROLLERS_AUXILIARY
