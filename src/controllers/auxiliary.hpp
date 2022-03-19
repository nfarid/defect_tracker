
#ifndef HPP_CONTROLLERS_AUXILIARY
#define HPP_CONTROLLERS_AUXILIARY

#include <drogon/Session.h>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <drogon/HttpViewData.h>

#include <functional>
#include <string>


// Some auxiliary functions for the controllers

namespace Aux
{


/**
 * \brief Obtains the session from a http request
 * \throws std::runtime_error if session is not enabled
 */
drogon::SessionPtr getSession(const drogon::HttpRequestPtr& req);

/**
 * \brief Obtain the data set to be sent to the view
 * @param title - the title of that the page should have
 * @param session - the session data
 */
drogon::HttpViewData getViewData(const std::string& title, const drogon::Session& session);


/**
 * \brief Check if the user is logged in
 */
bool isLoggedIn(const drogon::Session& session);

/**
 * \brief Creates the session when the user logs in
 */
void logIn(drogon::Session& session, int32_t userId, const std::string& username);


// A drogon callback function object that takes a http response from the controller
using ResponseCallback = std::function<void(const drogon::HttpResponsePtr&)>;


}  // namespace Aux

#endif  // ifndef HPP_CONTROLLERS_AUXILIARY
