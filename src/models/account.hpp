
#ifndef HPP_MODELS_ACCOUNT
#define HPP_MODELS_ACCOUNT

#include "../util/coroutine.hpp"
#include "../util/typedefs.hpp"
#include "generated_postgresql/account.hpp"


namespace Model
{


class Notification;


/**
 * @brief The Account class - an extension of the generated Account model with custom methods
 */
class Account : public drogon_model::bug_tracker::Account {
public:
    /**
     * @brief verifies the data from the login form
     * @param postParams - the data obtain from the POST request
     * @return the respective Account with the specified username
     * @throws Util::FormError if the data is incorrect
     */
    static drogon::Task<Account> verifyLogin(const Util::StringMap& postParams);

    /**
     * @brief validates the data from the signup form
     * @param postParams - the data obtain from the POST request
     * @return a newly created Account if the data is valid
     * @throws Util::FormError if the data is invalid
     */
    static drogon::Task<Account> createAccount(const Util::StringMap& postParams);


    drogon::Task<std::vector<Notification> > getNotifications() const;

    /**
     * @brief Turns a model into a Json suitable for the view (uses htmlTranslate)
     */
    Json::Value toViewJson() const;

private:
    using drogon_model::bug_tracker::Account::Account;

    friend drogon::orm::CoroMapper<Account>;
};


} // namespace Model

#endif  // ifndef HPP_MODELS_ACCOUNT
