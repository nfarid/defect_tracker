
#include "account.hpp"

#include "notification.hpp"

#include "../util/cstring_view.hpp"
#include "../util/database.hpp"
#include "../util/form_error.hpp"
#include "../util/string.hpp"

#include <drogon/HttpAppFramework.h>
#include <drogon/HttpViewData.h>

#include <sodium.h>


namespace Model
{


namespace
{


// Creates a hash from a password
std::string hash(Util::CStringView password) {
    std::string passwordHash(crypto_pwhash_STRBYTES, '\0');
    if(crypto_pwhash_str(passwordHash.data(), password.c_str(), size(password),
                crypto_pwhash_OPSLIMIT_MIN, crypto_pwhash_MEMLIMIT_MIN) )
    {
        throw std::runtime_error{"Hashing failed"};
    }
    return passwordHash;
}

// Checks if the hash matches against the password
bool verifyHash(Util::CStringView passwordHash, Util::CStringView password) {
    return !crypto_pwhash_str_verify(passwordHash.c_str(), password.c_str(), size(password) );
}


}  // namespace


using namespace drogon;
using namespace drogon::orm;


Task<Account> Account::verifyLogin(const Util::StringMap& postParams)
{
    CoroMapper<Account> orm = Util::getDb();

    // Obtain and trim the data from the POST request
    const std::string username = Util::getTrimmed(postParams.at("form-username") );
    const std::string password = Util::getTrimmed(postParams.at("form-password") );

    // Find a user with the specified username
    const Criteria hasUsername{Model::Account::Cols::_username, CompareOperator::EQ, username};
    const std::vector userLst = co_await orm.findBy(hasUsername);
    if(userLst.size() != 1) // No users with that username exists
        throw Util::FormError("Invalid username.");
    const Model::Account& user = userLst.front();

    // Check if the entered password is correct
    if(!verifyHash(user.getValueOfPasswordHash(), password) )
        throw Util::FormError("Invalid password.");

    // Verification is complete
    co_return user;
}

Task<Account> Account::createAccount(const Util::StringMap& postParams)
{
    CoroMapper<Account> orm = Util::getDb();

    // Obtain and trim the data from the POST request
    const std::string& username = Util::getTrimmed(postParams.at("form-username") );
    const std::string& password = Util::getTrimmed(postParams.at("form-password") );

    // TODO: Add more requirements
    if(username.empty() )
        throw Util::FormError("Username cannot be empty.");
    if(!Util::isAlNumUnderscore(username) )
        throw Util::FormError("Username can only contain alphabet, number or underscore.");
    if(username.size() > 30)
        throw Util::FormError("Username cannot be longer than 30 characters");
    if(password.empty() )
        throw Util::FormError("Password cannot be empty.");

    // Check if another user with that username already exists
    const Criteria hasUsername{Model::Account::Cols::_username, CompareOperator::EQ, username};
    const auto userCount = co_await orm.count(hasUsername);
    if(userCount != 0)
        throw Util::FormError("That username already exists, please pick another.");

    // Validation is complete, so create a new account
    Model::Account newAccount;
    newAccount.setUsername(username);
    newAccount.setPasswordHash(hash(password) );
    co_return co_await orm.insert(newAccount);
}

drogon::Task<std::vector<Notification> > Account::getNotifications() const {
    CoroMapper<Notification> notificationOrm = Util::getDb();
    const Criteria toUser{Notification::Cols::_user_id, CompareOperator::EQ, getValueOfId()};
    co_return co_await notificationOrm.findBy(toUser);
}

Json::Value Account::toViewJson() const {
    Json::Value json{};
    json["id"] = getValueOfId();
    json["username"] = HttpViewData::htmlTranslate(getValueOfUsername() );
    return json;
}


}  // namespace Model

