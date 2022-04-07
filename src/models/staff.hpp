
#include "../util/coroutine.hpp"
#include "generated/staff.hpp"


namespace Model
{


class Staff : public drogon_model::bug_tracker::Staff {
public:
    using drogon_model::bug_tracker::Staff::Staff;

    friend drogon::orm::CoroMapper<Staff>;
};


} // namespace Model

