
#ifndef HPP_MODELS_STAFF
#define HPP_MODELS_STAFF

#include "../util/coroutine.hpp"

#ifdef USE_POSTGRESQL
    #include "generated_postgresql/staff.hpp"
#else
    #include "generated_sqlite3/staff.hpp"
#endif  // ifndef USE_POSTGRESQL


namespace Model
{


/**
 * @brief The Staff class - an extension of the generated Staff model with custom methods
 */
class Staff : public drogon_model::Staff {
private:
    using drogon_model::Staff::Staff;

    friend drogon::orm::CoroMapper<Staff>;
};


} // namespace Model

#endif  // ifndef HPP_MODELS_STAFF
