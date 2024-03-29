
#include "account.hpp"

#include "notification.hpp"
#include "_database.hpp"

#include "../util/cstring_view.hpp"
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
                crypto_pwhash_OPSLIMIT_MIN, crypto_pwhash_MEMLIMIT_MIN) != 0)
    {
        throw std::runtime_error{"Hashing failed"};
    }
    return passwordHash;
}

// Checks if the hash matches against the password
bool verifyHash(Util::CStringView passwordHash, Util::CStringView password) {
    return crypto_pwhash_str_verify(passwordHash.c_str(), password.c_str(), size(password) ) == 0;
}


}  // namespace


using namespace drogon;
using namespace drogon::orm;
using Json::UInt64;


Task<Account> Account::verifyLogin(const Util::StringMap& formData)
{
    CoroMapper<Account> userOrm = Db::getDb();

    // Obtain and trim the data from the POST request
    const std::string username = Util::getTrimmed(formData.at("form-username") );
    const std::string password = Util::getTrimmed(formData.at("form-password") );

    // Find a user with the specified username
    const Criteria hasUsername{Model::Account::Cols::_username, CompareOperator::EQ, username};
    const std::vector userLst = co_await userOrm.findBy(hasUsername);
    if(userLst.size() != 1) // No users with that username exists
        throw Util::FormError("Invalid username.");
    const Model::Account& user = userLst.front();

    // Check if the entered password is correct
    if(!verifyHash(user.getValueOfPasswordHash(), password) )
        throw Util::FormError("Invalid password.");

    // Verification is complete
    co_return user;
}

Task<Account> Account::createAccount(const Util::StringMap& formData)
{
    CoroMapper<Account> userOrm = Db::getDb();

    // Obtain and trim the data from the POST request
    const std::string& username = Util::getTrimmed(formData.at("form-username") );
    const std::string& password = Util::getTrimmed(formData.at("form-password") );

    // Check if the submitted data is valid
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
    const auto userCount = co_await userOrm.count(hasUsername);
    if(userCount != 0)
        throw Util::FormError("That username already exists, please pick another.");

    // Validation is complete, so create a new account
    Model::Account newAccount;
    newAccount.setUsername(username);
    newAccount.setPasswordHash(hash(password) );
    co_return co_await userOrm.insert(newAccount);
}

drogon::Task<Account> Account::findByPrimaryKey(PrimaryKeyType userId) {
    CoroMapper<Account> userOrm = Db::getDb();
    co_return co_await userOrm.findByPrimaryKey(userId);
}

drogon::Task<Account> Account::findByUsername(const std::string& username) {
    CoroMapper<Account> userOrm = Db::getDb();
    const Criteria hasUsername{Model::Account::Cols::_username, CompareOperator::EQ, username};
    co_return co_await userOrm.findOne(hasUsername);
}

drogon::Task<std::vector<Notification> > Account::getNotifications() const {
    CoroMapper<Notification> notificationOrm = Db::getDb();
    const Criteria forUser{Notification::Cols::_user_id, CompareOperator::EQ, getValueOfId()};
    co_return co_await notificationOrm.findBy(forUser);
}

Json::Value Account::toViewJson() const {
    Json::Value json{};
    json["id"] = static_cast<UInt64>(getValueOfId() );
    json["username"] = HttpViewData::htmlTranslate(getValueOfUsername() );
    return json;
}


}  // namespace Model

