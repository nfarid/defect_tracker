
#ifndef HPP_APP
#define HPP_APP

#include <drogon/HttpAppFramework.h>

#include <string_view>

/**
 * @brief getApp - Create a drogon app
 */
drogon::HttpAppFramework& getApp(unsigned short port, std::string_view dbUrl);

#endif  // HPP_APP
