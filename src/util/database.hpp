
#ifndef HPP_UTIL_DATABASE
#define HPP_UTIL_DATABASE

#include <drogon/orm/DbClient.h>


namespace Util

{


// Obtain a pointer to a database client
drogon::orm::DbClientPtr getDb();


}  // namespace Util

#endif  // HPP_UTIL_DATABASE
