
#ifndef HPP_MODELS_ACCOUNT
#define HPP_MODELS_ACCOUNT

#include "../util/coroutine.hpp"
#include "../util/typedefs.hpp"

#ifdef USE_POSTGRESQL
    #include "generated_postgresql/account.hpp"
#else
    #include "generated_sqlite3/account.hpp"
#endif  // ifndef USE_POSTGRESQL


namespace Model
{


class Notification;


/**
 * @brief The Account class - an extension of the generated Account model with custom methods
 */
class Account : public drogon_model::Account {
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

    /**
     * @brief obtain the Account model that has the specified primary key
     * @throws drogon::orm::UnexpectedRows if it doesn't exists
     */
    static drogon::Task<Account> findByPrimaryKey(PrimaryKeyType userId);

    /**
     * @brief obtain the Account model that has the specified username
     * @throws drogon::orm::UnexpectedRows if it doesn't exists
     */
    static drogon::Task<Account> findByUsername(const std::string& username);


    drogon::Task<std::vector<Notification> > getNotifications() const;

    /**
     * @brief Turns a model into a Json suitable for the view (uses htmlTranslate)
     */
    Json::Value toViewJson() const;

private:
    using drogon_model::Account::Account;

    friend drogon::orm::CoroMapper<Account>;
};


} // namespace Model

#endif  // ifndef HPP_MODELS_ACCOUNT
