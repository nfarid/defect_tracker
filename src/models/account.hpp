
#include "../util/coroutine.hpp"
#include "../util/typedefs.hpp"
#include "generated/account.hpp"


namespace Model
{


class Account : public drogon_model::bug_tracker::Account {
public:
    /**
     * @brief verifies the data from the login form
     * @param orm  the object–relational mapper for Account
     * @param postParams  the data obtain from the POST request
     * @return the respective Account with the specified username
     * @throws Util::FormError if the data is incorrect
     */
    static drogon::Task<Account> verifyLogin(drogon::orm::CoroMapper<Account>& orm,
            const Util::StringMap& postParams);

    /**
     * @brief validates the data from the POST parameter
     * @param orm  the object–relational mapper for Account
     * @param postParams  the data obtain from the POST request
     * @return a newly created Account if the data is valid
     * @throws Util::FormError if the data is invalid
     */
    static drogon::Task<Account> createAccount(drogon::orm::CoroMapper<Account>& orm,
            const Util::StringMap& postParams);

    using drogon_model::bug_tracker::Account::Account;

    friend drogon::orm::CoroMapper<Account>;
};


} // namespace Model

