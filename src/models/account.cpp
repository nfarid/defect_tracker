
#include "account.hpp"

#include "../util/form_error.hpp"
#include "../util/hash.hpp"

#include <drogon/HttpAppFramework.h>
#include <drogon/HttpViewData.h>


namespace Model
{


using namespace drogon;
using namespace drogon::orm;


Task<Account> Account::verifyLogin(const Util::StringMap& postParams)
{
    CoroMapper<Account> orm = app().getDbClient("db");

    //Obtain the data from the POST request
    const std::string& username = postParams.at("form-username");
    const std::string password = postParams.at("form-password");

    //Find a user with the specified username
    const Criteria hasUsername{Model::Account::Cols::_username, CompareOperator::EQ, username};
    const std::vector userLst = co_await orm.findBy(hasUsername);
    if(userLst.size() != 1) //No users with that username exists
        throw Util::FormError("Invalid username.");
    const Model::Account& user = userLst.front();

    //Check if the entered password is correct
    if(!Util::verifyHash(user.getValueOfPasswordHash(), password) )
        throw Util::FormError("Invalid password.");

    //Verification is complete
    co_return user;
}

Task<Account> Account::createAccount(const Util::StringMap& postParams)
{
    CoroMapper<Account> orm = app().getDbClient("db");

    //Obtain the data from the POST request
    const std::string& username = postParams.at("form-username");
    const std::string& password = postParams.at("form-password");

    // TODO: Add more requirements for a valid username & password
    if(username.empty() )
        throw Util::FormError("Username cannot be empty.");
    if(password.empty() )
        throw Util::FormError("Password cannot be empty.");

    //Check if another user with that username already exists
    const Criteria hasUsername{Model::Account::Cols::_username, CompareOperator::EQ, username};
    const auto userCount = co_await orm.count(hasUsername);
    if(userCount != 0)
        throw Util::FormError("That username already exists, please pick another.");

    //Validation is complete, so create a new account
    Model::Account newAccount;
    newAccount.setUsername(username);
    newAccount.setPasswordHash(Util::hash(password) );
    co_return co_await orm.insert(newAccount);
}

Json::Value Account::toViewJson() const {
    Json::Value json{};
    json["id"] = getValueOfId();
    json["username"] = HttpViewData::htmlTranslate(getValueOfUsername() );
    return json;
}


}  // namespace Model

