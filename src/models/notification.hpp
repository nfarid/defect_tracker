
#ifndef HPP_MODELS_NOTIFICATIONS
#define HPP_MODELS_NOTIFICATIONS

#include "../util/coroutine.hpp"

#ifdef USE_POSTGRESQL
    #include "generated_postgresql/notification.hpp"
#else
    #include "generated_sqlite3/notification.hpp"
#endif  // ifndef USE_POSTGRESQL


namespace Model
{


/**
 * @brief The Notification class - an extension of the generated Notification model with custom methods
 */
class Notification : public drogon_model::Notification {
public:

    /**
     * @brief Turns a model into a Json suitable for the view (uses htmlTranslate)
     */
    Json::Value toViewJson() const;

private:
    using drogon_model::Notification::Notification;

    friend drogon::orm::CoroMapper<Notification>;
};


} // namespace Model

#endif  // ifndef HPP_MODELS_NOTIFICATIONS
