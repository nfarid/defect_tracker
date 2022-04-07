
#include "../util/coroutine.hpp"
#include "generated/notification.hpp"


namespace Model
{


class Notification : public drogon_model::bug_tracker::Notification {
public:
    using drogon_model::bug_tracker::Notification::Notification;

    friend drogon::orm::CoroMapper<Notification>;
};


} // namespace Model

