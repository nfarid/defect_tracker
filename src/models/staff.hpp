
#ifndef HPP_MODELS_STAFF
#define HPP_MODELS_STAFF

#include "../util/coroutine.hpp"
#include "generated_postgresql/staff.hpp"


namespace Model
{


/**
 * @brief The Staff class - an extension of the generated Staff model with custom methods
 */
class Staff : public drogon_model::bug_tracker::Staff {
public:
    using drogon_model::bug_tracker::Staff::Staff;

    friend drogon::orm::CoroMapper<Staff>;
};


} // namespace Model

#endif  // ifndef HPP_MODELS_STAFF
