
#ifndef HPP_CONTROLLERS_AUXILIARY
#define HPP_CONTROLLERS_AUXILIARY

#include "../util/typedefs.hpp"

#include <drogon/Session.h>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <drogon/HttpViewData.h>
#include <drogon/MultiPart.h>

#include <functional>
#include <string>


// Some auxiliary functions for the controllers

namespace Aux
{


using Util::IdType;

/**
 * @brief parseMultiPart - obtain the parameters from a multipart/form-data request
 * @param req - The POST request
 * @return 2 maps - one containing the regular inputs, and the other containing the file inputs
 * @throws Util::FormError if the httprequest cannot be parsed
 */
std::pair<Util::StringMap, Util::FileMap> parseMultiPart(const drogon::HttpRequestPtr& req);


/**
 * @brief Obtains the session from a http request
 * @throws std::runtime_error if session is not enabled
 */
drogon::SessionPtr getSession(const drogon::HttpRequestPtr& req);

/**
 * @brief Obtain the data set to be sent to the view
 * @param title - the title of that the page should have
 * @param session - the session data
 */
drogon::HttpViewData getViewData(const std::string& title, const drogon::Session& session);


/**
 * @brief Check if the user is logged in
 */
bool isLoggedIn(const drogon::Session& session);

/**
 * @brief obtain the user id from the session if logged in (returns zero otherwise)
 */
IdType getUserId(const drogon::Session& session);

/**
 * @brief obtain the username from the session if logged in (returns empty string otherwise)
 */
std::string getUsername(const drogon::Session& session);

/**
 * @brief Creates the session when the user logs in
 */
void logIn(drogon::Session& session, IdType userId, const std::string& username);

/**
 * @brief Clears out the session data for this user
 */
void logOut(drogon::Session& session);


/**
 * @brief Turns an array of elements into its json equivalent
 */
template<typename T>
Json::Value toViewJson(const std::vector<T>& lst) {
    Json::Value json{};
    for(const auto& elem : lst)
        json.append(elem.toViewJson() );
    return json;
}

// A drogon callback function object that takes a http response from the controller
using ResponseCallback = std::function<void(const drogon::HttpResponsePtr&)>;


}  // namespace Aux

#endif  // ifndef HPP_CONTROLLERS_AUXILIARY
