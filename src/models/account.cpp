
#include "account.hpp"

#include "../util/hash.hpp"


namespace Model
{


using namespace drogon;
using namespace drogon::orm;


Task<Account> Account::verifyLogin(CoroMapper<Account>& orm,
        const Util::StringMap& postParams)
{
    const std::string& username = postParams.at("form-username");
    const std::string password = postParams.at("form-password");

    const Criteria userCriteria{Model::Account::Cols::_username, CompareOperator::EQ, username};
    const Model::Account user = co_await orm.findOne(userCriteria);

    if(!Util::verifyHash(user.getValueOfPasswordHash(), password) )
        throw std::runtime_error("Form Error: Invalid password");

    co_return user;
}

Task<Account> Account::createAccount(CoroMapper<Account>& orm,
        const Util::StringMap& postParams)
{
    const std::string& username = postParams.at("form-username");
    const std::string& password = postParams.at("form-password");

    // TODO: Add more requirements for a valid username & password
    if(username.empty() || password.empty() ) // Username or password should not be empty
        throw std::runtime_error("Form Error: Invalid username or password");

    Model::Account newAccount;
    newAccount.setUsername(username);
    newAccount.setPasswordHash(Util::hash(password) );

    co_return co_await orm.insert(newAccount);
}


}  // namespace Model

