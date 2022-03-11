
// This file is generated by drogon_ctl

#include "./comment.hpp"
#include <drogon/utils/Utilities.h>
#include <string>

using namespace drogon;
using namespace drogon_model::bug_tracker;

const std::string Comment::Cols::_id = "id";
const std::string Comment::Cols::_post = "post";
const std::string Comment::Cols::_created_date = "created_date";
const std::string Comment::Cols::_project = "project";
const std::string Comment::primaryKeyName = "id";
const bool Comment::hasPrimaryKey = true;
const std::string Comment::tableName = "comment";

const std::vector<typename Comment::MetaData> Comment::metaData_={
    {"id", "int32_t", "integer", 4, true, true, true},
    {"post", "std::string", "text", 0, false, false, true},
    {"created_date", "trantor::Date", "timestamp without time zone", 0, false, false, true},
    {"project", "int32_t", "integer", 4, false, false, true}
};
const std::string& Comment::getColumnName(size_t index) noexcept(false)
{
    assert(index < metaData_.size() );
    return metaData_[index].colName_;
}

Comment::Comment(const Row& r, const ssize_t indexOffset) noexcept
{
    if(indexOffset < 0) {
        if(!r["id"].isNull() )
            id_=std::make_shared<int32_t>(r["id"].as<int32_t>() );
        if(!r["post"].isNull() )
            post_=std::make_shared<std::string>(r["post"].as<std::string>() );
        if(!r["created_date"].isNull() ) {
            auto timeStr = r["created_date"].as<std::string>();
            struct tm stm{};
            memset(&stm, 0, sizeof(stm) );
            auto p = strptime(timeStr.c_str(), "%Y-%m-%d %H:%M:%S", &stm);
            time_t t = mktime(&stm);
            size_t decimalNum = 0;
            if(p) {
                if(*p=='.') {
                    std::string decimals(p+1, &timeStr[timeStr.length()]);
                    while(decimals.length()<6)
                        decimals += "0";
                    decimalNum = static_cast<size_t>(atol(decimals.c_str() ) );
                }
                createdDate_=std::make_shared<trantor::Date>(t*1000000+decimalNum);
            }
        }
        if(!r["project"].isNull() )
            project_=std::make_shared<int32_t>(r["project"].as<int32_t>() );
    } else {
        size_t offset = static_cast<size_t>(indexOffset);
        if(offset + 4 > r.size() ) {
            LOG_FATAL << "Invalid SQL result for this model";
            return;
        }
        size_t index;
        index = offset + 0;
        if(!r[index].isNull() )
            id_=std::make_shared<int32_t>(r[index].as<int32_t>() );
        index = offset + 1;
        if(!r[index].isNull() )
            post_=std::make_shared<std::string>(r[index].as<std::string>() );
        index = offset + 2;
        if(!r[index].isNull() ) {
            auto timeStr = r[index].as<std::string>();
            struct tm stm{};
            memset(&stm, 0, sizeof(stm) );
            auto p = strptime(timeStr.c_str(), "%Y-%m-%d %H:%M:%S", &stm);
            time_t t = mktime(&stm);
            size_t decimalNum = 0;
            if(p) {
                if(*p=='.') {
                    std::string decimals(p+1, &timeStr[timeStr.length()]);
                    while(decimals.length()<6)
                        decimals += "0";
                    decimalNum = static_cast<size_t>(atol(decimals.c_str() ) );
                }
                createdDate_=std::make_shared<trantor::Date>(t*1000000+decimalNum);
            }
        }
        index = offset + 3;
        if(!r[index].isNull() )
            project_=std::make_shared<int32_t>(r[index].as<int32_t>() );
    }
}

Comment::Comment(const Json::Value& pJson, const std::vector<std::string>& pMasqueradingVector) noexcept(false)
{
    if(pMasqueradingVector.size() != 4) {
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
            post_=std::make_shared<std::string>(pJson[pMasqueradingVector[1]].asString() );
    }
    if(!pMasqueradingVector[2].empty() && pJson.isMember(pMasqueradingVector[2]) ) {
        dirtyFlag_[2] = true;
        if(!pJson[pMasqueradingVector[2]].isNull() ) {
            auto timeStr = pJson[pMasqueradingVector[2]].asString();
            struct tm stm{};
            memset(&stm, 0, sizeof(stm) );
            auto p = strptime(timeStr.c_str(), "%Y-%m-%d %H:%M:%S", &stm);
            time_t t = mktime(&stm);
            size_t decimalNum = 0;
            if(p) {
                if(*p=='.') {
                    std::string decimals(p+1, &timeStr[timeStr.length()]);
                    while(decimals.length()<6)
                        decimals += "0";
                    decimalNum = static_cast<size_t>(atol(decimals.c_str() ) );
                }
                createdDate_=std::make_shared<trantor::Date>(t*1000000+decimalNum);
            }
        }
    }
    if(!pMasqueradingVector[3].empty() && pJson.isMember(pMasqueradingVector[3]) ) {
        dirtyFlag_[3] = true;
        if(!pJson[pMasqueradingVector[3]].isNull() )
            project_=std::make_shared<int32_t>( static_cast<int32_t>(pJson[pMasqueradingVector[3]].asInt64() ) );
    }
}

Comment::Comment(const Json::Value& pJson) noexcept(false)
{
    if(pJson.isMember("id") ) {
        dirtyFlag_[0]=true;
        if(!pJson["id"].isNull() )
            id_=std::make_shared<int32_t>( static_cast<int32_t>(pJson["id"].asInt64() ) );
    }
    if(pJson.isMember("post") ) {
        dirtyFlag_[1]=true;
        if(!pJson["post"].isNull() )
            post_=std::make_shared<std::string>(pJson["post"].asString() );
    }
    if(pJson.isMember("created_date") ) {
        dirtyFlag_[2]=true;
        if(!pJson["created_date"].isNull() ) {
            auto timeStr = pJson["created_date"].asString();
            struct tm stm{};
            memset(&stm, 0, sizeof(stm) );
            auto p = strptime(timeStr.c_str(), "%Y-%m-%d %H:%M:%S", &stm);
            time_t t = mktime(&stm);
            size_t decimalNum = 0;
            if(p) {
                if(*p=='.') {
                    std::string decimals(p+1, &timeStr[timeStr.length()]);
                    while(decimals.length()<6)
                        decimals += "0";
                    decimalNum = static_cast<size_t>(atol(decimals.c_str() ) );
                }
                createdDate_=std::make_shared<trantor::Date>(t*1000000+decimalNum);
            }
        }
    }
    if(pJson.isMember("project") ) {
        dirtyFlag_[3]=true;
        if(!pJson["project"].isNull() )
            project_=std::make_shared<int32_t>( static_cast<int32_t>(pJson["project"].asInt64() ) );
    }
}

void Comment::updateByMasqueradedJson(const Json::Value& pJson,
        const std::vector<std::string>& pMasqueradingVector) noexcept(false)
{
    if(pMasqueradingVector.size() != 4) {
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
            post_=std::make_shared<std::string>(pJson[pMasqueradingVector[1]].asString() );
    }
    if(!pMasqueradingVector[2].empty() && pJson.isMember(pMasqueradingVector[2]) ) {
        dirtyFlag_[2] = true;
        if(!pJson[pMasqueradingVector[2]].isNull() ) {
            auto timeStr = pJson[pMasqueradingVector[2]].asString();
            struct tm stm{};
            memset(&stm, 0, sizeof(stm) );
            auto p = strptime(timeStr.c_str(), "%Y-%m-%d %H:%M:%S", &stm);
            time_t t = mktime(&stm);
            size_t decimalNum = 0;
            if(p) {
                if(*p=='.') {
                    std::string decimals(p+1, &timeStr[timeStr.length()]);
                    while(decimals.length()<6)
                        decimals += "0";
                    decimalNum = static_cast<size_t>(atol(decimals.c_str() ) );
                }
                createdDate_=std::make_shared<trantor::Date>(t*1000000+decimalNum);
            }
        }
    }
    if(!pMasqueradingVector[3].empty() && pJson.isMember(pMasqueradingVector[3]) ) {
        dirtyFlag_[3] = true;
        if(!pJson[pMasqueradingVector[3]].isNull() )
            project_=std::make_shared<int32_t>( static_cast<int32_t>(pJson[pMasqueradingVector[3]].asInt64() ) );
    }
}

void Comment::updateByJson(const Json::Value& pJson) noexcept(false)
{
    if(pJson.isMember("id") ) {
        if(!pJson["id"].isNull() )
            id_=std::make_shared<int32_t>( static_cast<int32_t>(pJson["id"].asInt64() ) );
    }
    if(pJson.isMember("post") ) {
        dirtyFlag_[1] = true;
        if(!pJson["post"].isNull() )
            post_=std::make_shared<std::string>(pJson["post"].asString() );
    }
    if(pJson.isMember("created_date") ) {
        dirtyFlag_[2] = true;
        if(!pJson["created_date"].isNull() ) {
            auto timeStr = pJson["created_date"].asString();
            struct tm stm{};
            memset(&stm, 0, sizeof(stm) );
            auto p = strptime(timeStr.c_str(), "%Y-%m-%d %H:%M:%S", &stm);
            time_t t = mktime(&stm);
            size_t decimalNum = 0;
            if(p) {
                if(*p=='.') {
                    std::string decimals(p+1, &timeStr[timeStr.length()]);
                    while(decimals.length()<6)
                        decimals += "0";
                    decimalNum = static_cast<size_t>(atol(decimals.c_str() ) );
                }
                createdDate_=std::make_shared<trantor::Date>(t*1000000+decimalNum);
            }
        }
    }
    if(pJson.isMember("project") ) {
        dirtyFlag_[3] = true;
        if(!pJson["project"].isNull() )
            project_=std::make_shared<int32_t>( static_cast<int32_t>(pJson["project"].asInt64() ) );
    }
}

const int32_t& Comment::getValueOfId() const noexcept
{
    const static int32_t defaultValue = int32_t();
    if(id_)
        return *id_;
    return defaultValue;
}

const std::shared_ptr<int32_t>& Comment::getId() const noexcept
{
    return id_;
}

void Comment::setId(const int32_t& pId) noexcept
{
    id_ = std::make_shared<int32_t>(pId);
    dirtyFlag_[0] = true;
}

const typename Comment::PrimaryKeyType& Comment::getPrimaryKey() const
{
    assert(id_);
    return *id_;
}

const std::string& Comment::getValueOfPost() const noexcept
{
    const static std::string defaultValue = std::string();
    if(post_)
        return *post_;
    return defaultValue;
}

const std::shared_ptr<std::string>& Comment::getPost() const noexcept
{
    return post_;
}

void Comment::setPost(const std::string& pPost) noexcept
{
    post_ = std::make_shared<std::string>(pPost);
    dirtyFlag_[1] = true;
}

void Comment::setPost(std::string&& pPost) noexcept
{
    post_ = std::make_shared<std::string>(std::move(pPost) );
    dirtyFlag_[1] = true;
}

const trantor::Date& Comment::getValueOfCreatedDate() const noexcept
{
    const static trantor::Date defaultValue = trantor::Date();
    if(createdDate_)
        return *createdDate_;
    return defaultValue;
}

const std::shared_ptr<trantor::Date>& Comment::getCreatedDate() const noexcept
{
    return createdDate_;
}

void Comment::setCreatedDate(const trantor::Date& pCreatedDate) noexcept
{
    createdDate_ = std::make_shared<trantor::Date>(pCreatedDate);
    dirtyFlag_[2] = true;
}

const int32_t& Comment::getValueOfProject() const noexcept
{
    const static int32_t defaultValue = int32_t();
    if(project_)
        return *project_;
    return defaultValue;
}

const std::shared_ptr<int32_t>& Comment::getProject() const noexcept
{
    return project_;
}

void Comment::setProject(const int32_t& pProject) noexcept
{
    project_ = std::make_shared<int32_t>(pProject);
    dirtyFlag_[3] = true;
}

void Comment::updateId(const uint64_t id)
{}

const std::vector<std::string>& Comment::insertColumns() noexcept
{
    static const std::vector<std::string> inCols={
        "post",
        "created_date",
        "project"
    };
    return inCols;
}

void Comment::outputArgs(drogon::orm::internal::SqlBinder& binder) const
{
    if(dirtyFlag_[1]) {
        if(getPost() )
            binder << getValueOfPost();
        else
            binder << nullptr;
    }
    if(dirtyFlag_[2]) {
        if(getCreatedDate() )
            binder << getValueOfCreatedDate();
        else
            binder << nullptr;
    }
    if(dirtyFlag_[3]) {
        if(getProject() )
            binder << getValueOfProject();
        else
            binder << nullptr;
    }
}

const std::vector<std::string> Comment::updateColumns() const
{
    std::vector<std::string> ret;
    if(dirtyFlag_[1])
        ret.push_back(getColumnName(1) );
    if(dirtyFlag_[2])
        ret.push_back(getColumnName(2) );
    if(dirtyFlag_[3])
        ret.push_back(getColumnName(3) );
    return ret;
}

void Comment::updateArgs(drogon::orm::internal::SqlBinder& binder) const
{
    if(dirtyFlag_[1]) {
        if(getPost() )
            binder << getValueOfPost();
        else
            binder << nullptr;
    }
    if(dirtyFlag_[2]) {
        if(getCreatedDate() )
            binder << getValueOfCreatedDate();
        else
            binder << nullptr;
    }
    if(dirtyFlag_[3]) {
        if(getProject() )
            binder << getValueOfProject();
        else
            binder << nullptr;
    }
}

Json::Value Comment::toJson() const
{
    Json::Value ret;
    if(getId() )
        ret["id"]=getValueOfId();
    else
        ret["id"]=Json::Value();
    if(getPost() )
        ret["post"]=getValueOfPost();
    else
        ret["post"]=Json::Value();
    if(getCreatedDate() )
        ret["created_date"]=getCreatedDate()->toDbStringLocal();
    else
        ret["created_date"]=Json::Value();
    if(getProject() )
        ret["project"]=getValueOfProject();
    else
        ret["project"]=Json::Value();
    return ret;
}

Json::Value Comment::toMasqueradedJson(
    const std::vector<std::string>& pMasqueradingVector) const
{
    Json::Value ret;
    if(pMasqueradingVector.size() == 4) {
        if(!pMasqueradingVector[0].empty() ) {
            if(getId() )
                ret[pMasqueradingVector[0]]=getValueOfId();
            else
                ret[pMasqueradingVector[0]]=Json::Value();
        }
        if(!pMasqueradingVector[1].empty() ) {
            if(getPost() )
                ret[pMasqueradingVector[1]]=getValueOfPost();
            else
                ret[pMasqueradingVector[1]]=Json::Value();
        }
        if(!pMasqueradingVector[2].empty() ) {
            if(getCreatedDate() )
                ret[pMasqueradingVector[2]]=getCreatedDate()->toDbStringLocal();
            else
                ret[pMasqueradingVector[2]]=Json::Value();
        }
        if(!pMasqueradingVector[3].empty() ) {
            if(getProject() )
                ret[pMasqueradingVector[3]]=getValueOfProject();
            else
                ret[pMasqueradingVector[3]]=Json::Value();
        }
        return ret;
    }
    LOG_ERROR << "Masquerade failed";
    if(getId() )
        ret["id"]=getValueOfId();
    else
        ret["id"]=Json::Value();
    if(getPost() )
        ret["post"]=getValueOfPost();
    else
        ret["post"]=Json::Value();
    if(getCreatedDate() )
        ret["created_date"]=getCreatedDate()->toDbStringLocal();
    else
        ret["created_date"]=Json::Value();
    if(getProject() )
        ret["project"]=getValueOfProject();
    else
        ret["project"]=Json::Value();
    return ret;
}

bool Comment::validateJsonForCreation(const Json::Value& pJson, std::string& err)
{
    if(pJson.isMember("id") ) {
        if(!validJsonOfField(0, "id", pJson["id"], err, true) )
            return false;
    }
    if(pJson.isMember("post") ) {
        if(!validJsonOfField(1, "post", pJson["post"], err, true) )
            return false;
    } else {
        err="The post column cannot be null";
        return false;
    }
    if(pJson.isMember("created_date") ) {
        if(!validJsonOfField(2, "created_date", pJson["created_date"], err, true) )
            return false;
    } else {
        err="The created_date column cannot be null";
        return false;
    }
    if(pJson.isMember("project") ) {
        if(!validJsonOfField(3, "project", pJson["project"], err, true) )
            return false;
    } else {
        err="The project column cannot be null";
        return false;
    }
    return true;
}

bool Comment::validateMasqueradedJsonForCreation(const Json::Value& pJson,
        const std::vector<std::string>& pMasqueradingVector,
        std::string& err)
{
    if(pMasqueradingVector.size() != 4) {
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
        if(!pMasqueradingVector[3].empty() ) {
            if(pJson.isMember(pMasqueradingVector[3]) ) {
                if(!validJsonOfField(3, pMasqueradingVector[3], pJson[pMasqueradingVector[3]], err, true) )
                    return false;
            } else {
                err="The " + pMasqueradingVector[3] + " column cannot be null";
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

bool Comment::validateJsonForUpdate(const Json::Value& pJson, std::string& err)
{
    if(pJson.isMember("id") ) {
        if(!validJsonOfField(0, "id", pJson["id"], err, false) )
            return false;
    } else {
        err = "The value of primary key must be set in the json object for update";
        return false;
    }
    if(pJson.isMember("post") ) {
        if(!validJsonOfField(1, "post", pJson["post"], err, false) )
            return false;
    }
    if(pJson.isMember("created_date") ) {
        if(!validJsonOfField(2, "created_date", pJson["created_date"], err, false) )
            return false;
    }
    if(pJson.isMember("project") ) {
        if(!validJsonOfField(3, "project", pJson["project"], err, false) )
            return false;
    }
    return true;
}

bool Comment::validateMasqueradedJsonForUpdate(const Json::Value& pJson,
        const std::vector<std::string>& pMasqueradingVector,
        std::string& err)
{
    if(pMasqueradingVector.size() != 4) {
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
        if(!pMasqueradingVector[3].empty() && pJson.isMember(pMasqueradingVector[3]) ) {
            if(!validJsonOfField(3, pMasqueradingVector[3], pJson[pMasqueradingVector[3]], err, false) )
                return false;
        }
    }catch(const Json::LogicError& e)
    {
        err = e.what();
        return false;
    }
    return true;
}

bool Comment::validJsonOfField(size_t index,
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
    case 3:
        if(pJson.isNull() ) {
            err="The " + fieldName + " column cannot be null";
            return false;
        }
        if(!pJson.isInt() ) {
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
