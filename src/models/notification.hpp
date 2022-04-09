
#ifndef HPP_MODELS_NOTIFICATIONS
#define HPP_MODELS_NOTIFICATIONS

#include "../util/coroutine.hpp"
#include "generated/notification.hpp"


namespace Model
{


/**
 * @brief The Notification class - an extension of the generated Notification model with custom methods
 */
class Notification : public drogon_model::bug_tracker::Notification {
public:
    using drogon_model::bug_tracker::Notification::Notification;

    friend drogon::orm::CoroMapper<Notification>;
};


} // namespace Model

#endif  // ifndef HPP_MODELS_NOTIFICATIONS
