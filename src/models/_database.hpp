
#ifndef HPP_MODELS_DATABASE_
#define HPP_MODELS_DATABASE_

#include <drogon/orm/DbClient.h>


namespace Db

{


// Obtain a pointer to a database client
drogon::orm::DbClientPtr getDb();


}  // namespace Db

#endif  // HPP_MODELS_DATABASE_
