
#ifndef HPP_UTIL_TYPEDEFS
#define HPP_UTIL_TYPEDEFS

#include "core.hpp"

#include <string>
#include <unordered_map>


/*NO-FORMAT*/

namespace drogon{class HttpFile;} //forward declaration


//Some type aliases for common types

namespace Util
{

#ifdef USE_POSTGRESQL
using IdType = std::int32_t;
#else
using IdType = std::uint64_t;
#endif

using StringMap = std::unordered_map<std::string, std::string>;

using FileMap = std::unordered_map<std::string, drogon::HttpFile>;

}  // namespace Util

/*YES-FORMAT*/

#endif  // ifndef HPP_UTIL_TYPEDEFS
