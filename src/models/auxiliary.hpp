
#ifndef HPP_MODELS_AUXILIARY
#define HPP_MODELS_AUXILIARY

#include "./account.hpp"
#include "./comment.hpp"

#include <drogon/orm/Mapper.h>
#include <json/value.h>

#include <array>
#include <string>



namespace Aux
{


using std::string_literals::operator""s;
using namespace drogon::orm;

// Finds all the comments that belong to a ticket with a specific id
// Throws
inline std::vector<Model::Comment> getTicketComments(int32_t ticketId, Mapper<Model::Comment>& orm)
{
    const Criteria commentCriteria{Model::Comment::Cols::_ticket_id, CompareOperator::EQ, ticketId};
    return orm.findBy(commentCriteria);
}

// Checks if the specified username already exist in the account table of database
inline bool isUsernameExist(const std::string& username, Mapper<Model::Account>& orm) {
    const Criteria userCriteria{Model::Account::Cols::_username, CompareOperator::EQ, username};
    return orm.count(userCriteria) > 0;
}

inline Model::Account createAccount(const std::string& username, const std::string& hash, Mapper<Model::Account>& orm)
{
    Model::Account newAccount;
    newAccount.setUsername(username);
    newAccount.setPasswordHash(hash);
    orm.insert(newAccount);  // This will also set the id for newAccount
    return newAccount;
}

// Finds a User that has the username
// Throws UnexpectedRows if none is found
inline Model::Account findByUsername(const std::string& username, Mapper<Model::Account>& orm) {
    const Criteria userCriteria{Model::Account::Cols::_username, CompareOperator::EQ, username};
    return orm.findOne(userCriteria);
}

const std::array statusLst = {"new"s, "confirmed"s, "unreproducible"s, "resolved"s, "reopened"s};
// Returns a list of statuses{"new","confirmed","unreproducible","resolved","reopened"} as a json
inline Json::Value statusLstJson() {
    const Json::Value static statusLst_ = []{
        Json::Value statusLst_;
        for(const auto& status : statusLst)
            statusLst_.append(status);
        return statusLst_;
    }();
    return statusLst_;
}

const std::array severityLst = {"low"s, "medium"s, "high"s};
// Returns a list of severities{"low","medium","high"} as a json
inline Json::Value severityLstJson() {
    const Json::Value static severityLst_ = []{
        Json::Value severityLst_;
        for(const auto& status : severityLst)
            severityLst_.append(status);
        return severityLst_;
    }();
    return severityLst_;
}


}  // namespace Aux

#endif  // ifndef HPP_MODELS_AUXILIARY
