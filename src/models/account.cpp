
// This file is generated by drogon_ctl

#include "./account.hpp"
#include <drogon/utils/Utilities.h>
#include <string>

using namespace drogon;
using namespace drogon_model::bug_tracker;

const std::string Account::Cols::_id = "id";
const std::string Account::Cols::_username = "username";
const std::string Account::Cols::_password_digest = "password_digest";
const std::string Account::primaryKeyName = "id";
const bool Account::hasPrimaryKey = true;
const std::string Account::tableName = "account";

const std::vector<typename Account::MetaData> Account::metaData_={
    {"id", "int32_t", "integer", 4, true, true, true},
    {"username", "std::string", "text", 0, false, false, true},
    {"password_digest", "std::string", "text", 0, false, false, true}
};
const std::string& Account::getColumnName(size_t index) noexcept(false)
{
    assert(index < metaData_.size() );
    return metaData_[index].colName_;
}

Account::Account(const Row& r, const ssize_t indexOffset) noexcept
{
    if(indexOffset < 0) {
        if(!r["id"].isNull() )
            id_=std::make_shared<int32_t>(r["id"].as<int32_t>() );
        if(!r["username"].isNull() )
            username_=std::make_shared<std::string>(r["username"].as<std::string>() );
        if(!r["password_digest"].isNull() )
            passwordDigest_=std::make_shared<std::string>(r["password_digest"].as<std::string>() );
    } else {
        size_t offset = static_cast<size_t>(indexOffset);
        if(offset + 3 > r.size() ) {
            LOG_FATAL << "Invalid SQL result for this model";
            return;
        }
        size_t index;
        index = offset + 0;
        if(!r[index].isNull() )
            id_=std::make_shared<int32_t>(r[index].as<int32_t>() );
        index = offset + 1;
        if(!r[index].isNull() )
            username_=std::make_shared<std::string>(r[index].as<std::string>() );
        index = offset + 2;
        if(!r[index].isNull() )
            passwordDigest_=std::make_shared<std::string>(r[index].as<std::string>() );
    }
}

Account::Account(const Json::Value& pJson, const std::vector<std::string>& pMasqueradingVector) noexcept(false)
{
    if(pMasqueradingVector.size() != 3) {
        LOG_ERROR << "Bad masquerading vector";
        return;
    }
    if(!pMasqueradingVector[0].empty() && pJson.isMember(pMasqueradingVector[0]) ) {
        dirtyFlag_[0] = true;
        if(!pJson[pMasqueradingVector[0]].isNull() )
            id_=std::make_shared<int32_t>( static_cast<int32_t>(pJson[pMasqueradingVector[0]].asInt64() ) );
    }
    if(!pMasqueradingVector[1].empty() && pJson.isMember(pMasqueradingVector[1]) ) {
        dirtyFlag_[1] = true;
        if(!pJson[pMasqueradingVector[1]].isNull() )
            username_=std::make_shared<std::string>(pJson[pMasqueradingVector[1]].asString() );
    }
    if(!pMasqueradingVector[2].empty() && pJson.isMember(pMasqueradingVector[2]) ) {
        dirtyFlag_[2] = true;
        if(!pJson[pMasqueradingVector[2]].isNull() )
            passwordDigest_=std::make_shared<std::string>(pJson[pMasqueradingVector[2]].asString() );
    }
}

Account::Account(const Json::Value& pJson) noexcept(false)
{
    if(pJson.isMember("id") ) {
        dirtyFlag_[0]=true;
        if(!pJson["id"].isNull() )
            id_=std::make_shared<int32_t>( static_cast<int32_t>(pJson["id"].asInt64() ) );
    }
    if(pJson.isMember("username") ) {
        dirtyFlag_[1]=true;
        if(!pJson["username"].isNull() )
            username_=std::make_shared<std::string>(pJson["username"].asString() );
    }
    if(pJson.isMember("password_digest") ) {
        dirtyFlag_[2]=true;
        if(!pJson["password_digest"].isNull() )
            passwordDigest_=std::make_shared<std::string>(pJson["password_digest"].asString() );
    }
}

void Account::updateByMasqueradedJson(const Json::Value& pJson,
        const std::vector<std::string>& pMasqueradingVector) noexcept(false)
{
    if(pMasqueradingVector.size() != 3) {
        LOG_ERROR << "Bad masquerading vector";
        return;
    }
    if(!pMasqueradingVector[0].empty() && pJson.isMember(pMasqueradingVector[0]) ) {
        if(!pJson[pMasqueradingVector[0]].isNull() )
            id_=std::make_shared<int32_t>( static_cast<int32_t>(pJson[pMasqueradingVector[0]].asInt64() ) );
    }
    if(!pMasqueradingVector[1].empty() && pJson.isMember(pMasqueradingVector[1]) ) {
        dirtyFlag_[1] = true;
        if(!pJson[pMasqueradingVector[1]].isNull() )
            username_=std::make_shared<std::string>(pJson[pMasqueradingVector[1]].asString() );
    }
    if(!pMasqueradingVector[2].empty() && pJson.isMember(pMasqueradingVector[2]) ) {
        dirtyFlag_[2] = true;
        if(!pJson[pMasqueradingVector[2]].isNull() )
            passwordDigest_=std::make_shared<std::string>(pJson[pMasqueradingVector[2]].asString() );
    }
}

void Account::updateByJson(const Json::Value& pJson) noexcept(false)
{
    if(pJson.isMember("id") ) {
        if(!pJson["id"].isNull() )
            id_=std::make_shared<int32_t>( static_cast<int32_t>(pJson["id"].asInt64() ) );
    }
    if(pJson.isMember("username") ) {
        dirtyFlag_[1] = true;
        if(!pJson["username"].isNull() )
            username_=std::make_shared<std::string>(pJson["username"].asString() );
    }
    if(pJson.isMember("password_digest") ) {
        dirtyFlag_[2] = true;
        if(!pJson["password_digest"].isNull() )
            passwordDigest_=std::make_shared<std::string>(pJson["password_digest"].asString() );
    }
}

const int32_t& Account::getValueOfId() const noexcept
{
    const static int32_t defaultValue = int32_t();
    if(id_)
        return *id_;
    return defaultValue;
}

const std::shared_ptr<int32_t>& Account::getId() const noexcept
{
    return id_;
}

void Account::setId(const int32_t& pId) noexcept
{
    id_ = std::make_shared<int32_t>(pId);
    dirtyFlag_[0] = true;
}

const typename Account::PrimaryKeyType& Account::getPrimaryKey() const
{
    assert(id_);
    return *id_;
}

const std::string& Account::getValueOfUsername() const noexcept
{
    const static std::string defaultValue = std::string();
    if(username_)
        return *username_;
    return defaultValue;
}

const std::shared_ptr<std::string>& Account::getUsername() const noexcept
{
    return username_;
}

void Account::setUsername(const std::string& pUsername) noexcept
{
    username_ = std::make_shared<std::string>(pUsername);
    dirtyFlag_[1] = true;
}

void Account::setUsername(std::string&& pUsername) noexcept
{
    username_ = std::make_shared<std::string>(std::move(pUsername) );
    dirtyFlag_[1] = true;
}

const std::string& Account::getValueOfPasswordDigest() const noexcept
{
    const static std::string defaultValue = std::string();
    if(passwordDigest_)
        return *passwordDigest_;
    return defaultValue;
}

const std::shared_ptr<std::string>& Account::getPasswordDigest() const noexcept
{
    return passwordDigest_;
}

void Account::setPasswordDigest(const std::string& pPasswordDigest) noexcept
{
    passwordDigest_ = std::make_shared<std::string>(pPasswordDigest);
    dirtyFlag_[2] = true;
}

void Account::setPasswordDigest(std::string&& pPasswordDigest) noexcept
{
    passwordDigest_ = std::make_shared<std::string>(std::move(pPasswordDigest) );
    dirtyFlag_[2] = true;
}

void Account::updateId(const uint64_t id)
{}

const std::vector<std::string>& Account::insertColumns() noexcept
{
    static const std::vector<std::string> inCols={
        "username",
        "password_digest"
    };
    return inCols;
}

void Account::outputArgs(drogon::orm::internal::SqlBinder& binder) const
{
    if(dirtyFlag_[1]) {
        if(getUsername() )
            binder << getValueOfUsername();
        else
            binder << nullptr;
    }
    if(dirtyFlag_[2]) {
        if(getPasswordDigest() )
            binder << getValueOfPasswordDigest();
        else
            binder << nullptr;
    }
}

const std::vector<std::string> Account::updateColumns() const
{
    std::vector<std::string> ret;
    if(dirtyFlag_[1])
        ret.push_back(getColumnName(1) );
    if(dirtyFlag_[2])
        ret.push_back(getColumnName(2) );
    return ret;
}

void Account::updateArgs(drogon::orm::internal::SqlBinder& binder) const
{
    if(dirtyFlag_[1]) {
        if(getUsername() )
            binder << getValueOfUsername();
        else
            binder << nullptr;
    }
    if(dirtyFlag_[2]) {
        if(getPasswordDigest() )
            binder << getValueOfPasswordDigest();
        else
            binder << nullptr;
    }
}

Json::Value Account::toJson() const
{
    Json::Value ret;
    if(getId() )
        ret["id"]=getValueOfId();
    else
        ret["id"]=Json::Value();
    if(getUsername() )
        ret["username"]=getValueOfUsername();
    else
        ret["username"]=Json::Value();
    if(getPasswordDigest() )
        ret["password_digest"]=getValueOfPasswordDigest();
    else
        ret["password_digest"]=Json::Value();
    return ret;
}

Json::Value Account::toMasqueradedJson(
    const std::vector<std::string>& pMasqueradingVector) const
{
    Json::Value ret;
    if(pMasqueradingVector.size() == 3) {
        if(!pMasqueradingVector[0].empty() ) {
            if(getId() )
                ret[pMasqueradingVector[0]]=getValueOfId();
            else
                ret[pMasqueradingVector[0]]=Json::Value();
        }
        if(!pMasqueradingVector[1].empty() ) {
            if(getUsername() )
                ret[pMasqueradingVector[1]]=getValueOfUsername();
            else
                ret[pMasqueradingVector[1]]=Json::Value();
        }
        if(!pMasqueradingVector[2].empty() ) {
            if(getPasswordDigest() )
                ret[pMasqueradingVector[2]]=getValueOfPasswordDigest();
            else
                ret[pMasqueradingVector[2]]=Json::Value();
        }
        return ret;
    }
    LOG_ERROR << "Masquerade failed";
    if(getId() )
        ret["id"]=getValueOfId();
    else
        ret["id"]=Json::Value();
    if(getUsername() )
        ret["username"]=getValueOfUsername();
    else
        ret["username"]=Json::Value();
    if(getPasswordDigest() )
        ret["password_digest"]=getValueOfPasswordDigest();
    else
        ret["password_digest"]=Json::Value();
    return ret;
}

bool Account::validateJsonForCreation(const Json::Value& pJson, std::string& err)
{
    if(pJson.isMember("id") ) {
        if(!validJsonOfField(0, "id", pJson["id"], err, true) )
            return false;
    }
    if(pJson.isMember("username") ) {
        if(!validJsonOfField(1, "username", pJson["username"], err, true) )
            return false;
    } else {
        err="The username column cannot be null";
        return false;
    }
    if(pJson.isMember("password_digest") ) {
        if(!validJsonOfField(2, "password_digest", pJson["password_digest"], err, true) )
            return false;
    } else {
        err="The password_digest column cannot be null";
        return false;
    }
    return true;
}

bool Account::validateMasqueradedJsonForCreation(const Json::Value& pJson,
        const std::vector<std::string>& pMasqueradingVector,
        std::string& err)
{
    if(pMasqueradingVector.size() != 3) {
        err = "Bad masquerading vector";
        return false;
    }
    try {
        if(!pMasqueradingVector[0].empty() ) {
            if(pJson.isMember(pMasqueradingVector[0]) ) {
                if(!validJsonOfField(0, pMasqueradingVector[0], pJson[pMasqueradingVector[0]], err, true) )
                    return false;
            }
        }
        if(!pMasqueradingVector[1].empty() ) {
            if(pJson.isMember(pMasqueradingVector[1]) ) {
                if(!validJsonOfField(1, pMasqueradingVector[1], pJson[pMasqueradingVector[1]], err, true) )
                    return false;
            } else {
                err="The " + pMasqueradingVector[1] + " column cannot be null";
                return false;
            }
        }
        if(!pMasqueradingVector[2].empty() ) {
            if(pJson.isMember(pMasqueradingVector[2]) ) {
                if(!validJsonOfField(2, pMasqueradingVector[2], pJson[pMasqueradingVector[2]], err, true) )
                    return false;
            } else {
                err="The " + pMasqueradingVector[2] + " column cannot be null";
                return false;
            }
        }
    }catch(const Json::LogicError& e)
    {
        err = e.what();
        return false;
    }
    return true;
}

bool Account::validateJsonForUpdate(const Json::Value& pJson, std::string& err)
{
    if(pJson.isMember("id") ) {
        if(!validJsonOfField(0, "id", pJson["id"], err, false) )
            return false;
    } else {
        err = "The value of primary key must be set in the json object for update";
        return false;
    }
    if(pJson.isMember("username") ) {
        if(!validJsonOfField(1, "username", pJson["username"], err, false) )
            return false;
    }
    if(pJson.isMember("password_digest") ) {
        if(!validJsonOfField(2, "password_digest", pJson["password_digest"], err, false) )
            return false;
    }
    return true;
}

bool Account::validateMasqueradedJsonForUpdate(const Json::Value& pJson,
        const std::vector<std::string>& pMasqueradingVector,
        std::string& err)
{
    if(pMasqueradingVector.size() != 3) {
        err = "Bad masquerading vector";
        return false;
    }
    try {
        if(!pMasqueradingVector[0].empty() && pJson.isMember(pMasqueradingVector[0]) ) {
            if(!validJsonOfField(0, pMasqueradingVector[0], pJson[pMasqueradingVector[0]], err, false) )
                return false;
        } else {
            err = "The value of primary key must be set in the json object for update";
            return false;
        }
        if(!pMasqueradingVector[1].empty() && pJson.isMember(pMasqueradingVector[1]) ) {
            if(!validJsonOfField(1, pMasqueradingVector[1], pJson[pMasqueradingVector[1]], err, false) )
                return false;
        }
        if(!pMasqueradingVector[2].empty() && pJson.isMember(pMasqueradingVector[2]) ) {
            if(!validJsonOfField(2, pMasqueradingVector[2], pJson[pMasqueradingVector[2]], err, false) )
                return false;
        }
    }catch(const Json::LogicError& e)
    {
        err = e.what();
        return false;
    }
    return true;
}

bool Account::validJsonOfField(size_t index,
        const std::string& fieldName,
        const Json::Value& pJson,
        std::string& err,
        bool isForCreation)
{
    switch(index) {
    case 0:
        if(pJson.isNull() ) {
            err="The " + fieldName + " column cannot be null";
            return false;
        }
        if(isForCreation) {
            err="The automatic primary key cannot be set";
            return false;
        }
        if(!pJson.isInt() ) {
            err="Type error in the "+fieldName+" field";
            return false;
        }
        break;
    case 1:
        if(pJson.isNull() ) {
            err="The " + fieldName + " column cannot be null";
            return false;
        }
        if(!pJson.isString() ) {
            err="Type error in the "+fieldName+" field";
            return false;
        }
        break;
    case 2:
        if(pJson.isNull() ) {
            err="The " + fieldName + " column cannot be null";
            return false;
        }
        if(!pJson.isString() ) {
            err="Type error in the "+fieldName+" field";
            return false;
        }
        break;

    default:
        err="Internal error in the server";
        return false;
        break;
    }
    return true;
}
