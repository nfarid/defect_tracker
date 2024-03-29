
#ifndef HPP_UTIL_CONSTANTS
#define HPP_UTIL_CONSTANTS


namespace Util
{


// This is port that the app under testing will be listening to
constexpr unsigned short testPort = 3000;

// The ip address for local host
constexpr const char* localhost = "127.0.0.1";

// Date format is day/month/year hour:minute (e.g. 10/March/2022 21:23)
constexpr const char* dateFormat = "%e/%b/%Y %H:%M";


}  // namespace Util

#endif  // ifndef HPP_UTIL_CONSTANTS
