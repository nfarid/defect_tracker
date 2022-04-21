
// This file is generated by drogon_ctl

#include "project.hpp"

#include "account.hpp"
#include "staff.hpp"
#include "ticket.hpp"

#include <drogon/utils/Utilities.h>

#include <string>

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model;

const std::string Project::Cols::_id = "id";
const std::string Project::Cols::_title = "title";
const std::string Project::Cols::_description = "description";
const std::string Project::Cols::_manager_id = "manager_id";
const std::string Project::primaryKeyName = "id";
const bool Project::hasPrimaryKey = true;
const std::string Project::tableName = "project";

const std::vector<typename Project::MetaData> Project::metaData_={
    {"id", "uint64_t", "integer", 8, 1, 1, 0},
    {"title", "std::string", "text", 0, 0, 0, 1},
    {"description", "std::string", "text", 0, 0, 0, 1},
    {"manager_id", "uint64_t", "integer", 8, 0, 0, 0}
};
const std::string& Project::getColumnName(size_t index) noexcept(false)
{
    assert(index < metaData_.size() );
    return metaData_[index].colName_;
}

Project::Project(const Row& r, const ssize_t indexOffset) noexcept
{
    if(indexOffset < 0) {
        if(!r["id"].isNull() )
            id_=std::make_shared<uint64_t>(r["id"].as<uint64_t>() );
        if(!r["title"].isNull() )
            title_=std::make_shared<std::string>(r["title"].as<std::string>() );
        if(!r["description"].isNull() )
            description_=std::make_shared<std::string>(r["description"].as<std::string>() );
        if(!r["manager_id"].isNull() )
            managerId_=std::make_shared<uint64_t>(r["manager_id"].as<uint64_t>() );
    } else{
        size_t offset = (size_t)indexOffset;
        if(offset + 4 > r.size() ) {
            LOG_FATAL << "Invalid SQL result for this model";
            return;
        }
        size_t index;
        index = offset + 0;
        if(!r[index].isNull() )
            id_=std::make_shared<uint64_t>(r[index].as<uint64_t>() );
        index = offset + 1;
        if(!r[index].isNull() )
            title_=std::make_shared<std::string>(r[index].as<std::string>() );
        index = offset + 2;
        if(!r[index].isNull() )
            description_=std::make_shared<std::string>(r[index].as<std::string>() );
        index = offset + 3;
        if(!r[index].isNull() )
            managerId_=std::make_shared<uint64_t>(r[index].as<uint64_t>() );
    }
}

Project::Project(const Json::Value& pJson, const std::vector<std::string>& pMasqueradingVector) noexcept(false)
{
    if(pMasqueradingVector.size() != 4) {
        LOG_ERROR << "Bad masquerading vector";
        return;
    }
    if(!pMasqueradingVector[0].empty() && pJson.isMember(pMasqueradingVector[0]) ) {
        dirtyFlag_[0] = true;
        if(!pJson[pMasqueradingVector[0]].isNull() )
            id_=std::make_shared<uint64_t>( (uint64_t)pJson[pMasqueradingVector[0]].asUInt64() );
    }
    if(!pMasqueradingVector[1].empty() && pJson.isMember(pMasqueradingVector[1]) ) {
        dirtyFlag_[1] = true;
        if(!pJson[pMasqueradingVector[1]].isNull() )
            title_=std::make_shared<std::string>(pJson[pMasqueradingVector[1]].asString() );
    }
    if(!pMasqueradingVector[2].empty() && pJson.isMember(pMasqueradingVector[2]) ) {
        dirtyFlag_[2] = true;
        if(!pJson[pMasqueradingVector[2]].isNull() )
            description_=std::make_shared<std::string>(pJson[pMasqueradingVector[2]].asString() );
    }
    if(!pMasqueradingVector[3].empty() && pJson.isMember(pMasqueradingVector[3]) ) {
        dirtyFlag_[3] = true;
        if(!pJson[pMasqueradingVector[3]].isNull() )
            managerId_=std::make_shared<uint64_t>( (uint64_t)pJson[pMasqueradingVector[3]].asUInt64() );
    }
}

Project::Project(const Json::Value& pJson) noexcept(false)
{
    if(pJson.isMember("id") ) {
        dirtyFlag_[0]=true;
        if(!pJson["id"].isNull() )
            id_=std::make_shared<uint64_t>( (uint64_t)pJson["id"].asUInt64() );
    }
    if(pJson.isMember("title") ) {
        dirtyFlag_[1]=true;
        if(!pJson["title"].isNull() )
            title_=std::make_shared<std::string>(pJson["title"].asString() );
    }
    if(pJson.isMember("description") ) {
        dirtyFlag_[2]=true;
        if(!pJson["description"].isNull() )
            description_=std::make_shared<std::string>(pJson["description"].asString() );
    }
    if(pJson.isMember("manager_id") ) {
        dirtyFlag_[3]=true;
        if(!pJson["manager_id"].isNull() )
            managerId_=std::make_shared<uint64_t>( (uint64_t)pJson["manager_id"].asUInt64() );
    }
}

void Project::updateByMasqueradedJson(const Json::Value& pJson,
        const std::vector<std::string>& pMasqueradingVector) noexcept(false)
{
    if(pMasqueradingVector.size() != 4) {
        LOG_ERROR << "Bad masquerading vector";
        return;
    }
    if(!pMasqueradingVector[0].empty() && pJson.isMember(pMasqueradingVector[0]) ) {
        if(!pJson[pMasqueradingVector[0]].isNull() )
            id_=std::make_shared<uint64_t>( (uint64_t)pJson[pMasqueradingVector[0]].asUInt64() );
    }
    if(!pMasqueradingVector[1].empty() && pJson.isMember(pMasqueradingVector[1]) ) {
        dirtyFlag_[1] = true;
        if(!pJson[pMasqueradingVector[1]].isNull() )
            title_=std::make_shared<std::string>(pJson[pMasqueradingVector[1]].asString() );
    }
    if(!pMasqueradingVector[2].empty() && pJson.isMember(pMasqueradingVector[2]) ) {
        dirtyFlag_[2] = true;
        if(!pJson[pMasqueradingVector[2]].isNull() )
            description_=std::make_shared<std::string>(pJson[pMasqueradingVector[2]].asString() );
    }
    if(!pMasqueradingVector[3].empty() && pJson.isMember(pMasqueradingVector[3]) ) {
        dirtyFlag_[3] = true;
        if(!pJson[pMasqueradingVector[3]].isNull() )
            managerId_=std::make_shared<uint64_t>( (uint64_t)pJson[pMasqueradingVector[3]].asUInt64() );
    }
}

void Project::updateByJson(const Json::Value& pJson) noexcept(false)
{
    if(pJson.isMember("id") ) {
        if(!pJson["id"].isNull() )
            id_=std::make_shared<uint64_t>( (uint64_t)pJson["id"].asUInt64() );
    }
    if(pJson.isMember("title") ) {
        dirtyFlag_[1] = true;
        if(!pJson["title"].isNull() )
            title_=std::make_shared<std::string>(pJson["title"].asString() );
    }
    if(pJson.isMember("description") ) {
        dirtyFlag_[2] = true;
        if(!pJson["description"].isNull() )
            description_=std::make_shared<std::string>(pJson["description"].asString() );
    }
    if(pJson.isMember("manager_id") ) {
        dirtyFlag_[3] = true;
        if(!pJson["manager_id"].isNull() )
            managerId_=std::make_shared<uint64_t>( (uint64_t)pJson["manager_id"].asUInt64() );
    }
}

const uint64_t& Project::getValueOfId() const noexcept
{
    const static uint64_t defaultValue = uint64_t();
    if(id_)
        return *id_;
    return defaultValue;
}

const std::shared_ptr<uint64_t>& Project::getId() const noexcept
{
    return id_;
}

void Project::setId(const uint64_t& pId) noexcept
{
    id_ = std::make_shared<uint64_t>(pId);
    dirtyFlag_[0] = true;
}

void Project::setIdToNull() noexcept
{
    id_.reset();
    dirtyFlag_[0] = true;
}

const typename Project::PrimaryKeyType& Project::getPrimaryKey() const
{
    assert(id_);
    return *id_;
}

const std::string& Project::getValueOfTitle() const noexcept
{
    const static std::string defaultValue = std::string();
    if(title_)
        return *title_;
    return defaultValue;
}

const std::shared_ptr<std::string>& Project::getTitle() const noexcept
{
    return title_;
}

void Project::setTitle(const std::string& pTitle) noexcept
{
    title_ = std::make_shared<std::string>(pTitle);
    dirtyFlag_[1] = true;
}

void Project::setTitle(std::string&& pTitle) noexcept
{
    title_ = std::make_shared<std::string>(std::move(pTitle) );
    dirtyFlag_[1] = true;
}

const std::string& Project::getValueOfDescription() const noexcept
{
    const static std::string defaultValue = std::string();
    if(description_)
        return *description_;
    return defaultValue;
}

const std::shared_ptr<std::string>& Project::getDescription() const noexcept
{
    return description_;
}

void Project::setDescription(const std::string& pDescription) noexcept
{
    description_ = std::make_shared<std::string>(pDescription);
    dirtyFlag_[2] = true;
}

void Project::setDescription(std::string&& pDescription) noexcept
{
    description_ = std::make_shared<std::string>(std::move(pDescription) );
    dirtyFlag_[2] = true;
}

const uint64_t& Project::getValueOfManagerId() const noexcept
{
    const static uint64_t defaultValue = uint64_t();
    if(managerId_)
        return *managerId_;
    return defaultValue;
}

const std::shared_ptr<uint64_t>& Project::getManagerId() const noexcept
{
    return managerId_;
}

void Project::setManagerId(const uint64_t& pManagerId) noexcept
{
    managerId_ = std::make_shared<uint64_t>(pManagerId);
    dirtyFlag_[3] = true;
}

void Project::setManagerIdToNull() noexcept
{
    managerId_.reset();
    dirtyFlag_[3] = true;
}

void Project::updateId(const uint64_t id)
{
    id_ = std::make_shared<uint64_t>(id);
}

const std::vector<std::string>& Project::insertColumns() noexcept
{
    static const std::vector<std::string> inCols={
        "title",
        "description",
        "manager_id"
    };
    return inCols;
}

void Project::outputArgs(drogon::orm::internal::SqlBinder& binder) const
{
    if(dirtyFlag_[1]) {
        if(getTitle() )
            binder << getValueOfTitle();
        else
            binder << nullptr;
    }
    if(dirtyFlag_[2]) {
        if(getDescription() )
            binder << getValueOfDescription();
        else
            binder << nullptr;
    }
    if(dirtyFlag_[3]) {
        if(getManagerId() )
            binder << getValueOfManagerId();
        else
            binder << nullptr;
    }
}

const std::vector<std::string> Project::updateColumns() const
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

void Project::updateArgs(drogon::orm::internal::SqlBinder& binder) const
{
    if(dirtyFlag_[1]) {
        if(getTitle() )
            binder << getValueOfTitle();
        else
            binder << nullptr;
    }
    if(dirtyFlag_[2]) {
        if(getDescription() )
            binder << getValueOfDescription();
        else
            binder << nullptr;
    }
    if(dirtyFlag_[3]) {
        if(getManagerId() )
            binder << getValueOfManagerId();
        else
            binder << nullptr;
    }
}

Json::Value Project::toJson() const
{
    Json::Value ret;
    if(getId() )
        ret["id"]=(Json::UInt64)getValueOfId();
    else
        ret["id"]=Json::Value();
    if(getTitle() )
        ret["title"]=getValueOfTitle();
    else
        ret["title"]=Json::Value();
    if(getDescription() )
        ret["description"]=getValueOfDescription();
    else
        ret["description"]=Json::Value();
    if(getManagerId() )
        ret["manager_id"]=(Json::UInt64)getValueOfManagerId();
    else
        ret["manager_id"]=Json::Value();
    return ret;
}

Json::Value Project::toMasqueradedJson(
    const std::vector<std::string>& pMasqueradingVector) const
{
    Json::Value ret;
    if(pMasqueradingVector.size() == 4) {
        if(!pMasqueradingVector[0].empty() ) {
            if(getId() )
                ret[pMasqueradingVector[0]]=(Json::UInt64)getValueOfId();
            else
                ret[pMasqueradingVector[0]]=Json::Value();
        }
        if(!pMasqueradingVector[1].empty() ) {
            if(getTitle() )
                ret[pMasqueradingVector[1]]=getValueOfTitle();
            else
                ret[pMasqueradingVector[1]]=Json::Value();
        }
        if(!pMasqueradingVector[2].empty() ) {
            if(getDescription() )
                ret[pMasqueradingVector[2]]=getValueOfDescription();
            else
                ret[pMasqueradingVector[2]]=Json::Value();
        }
        if(!pMasqueradingVector[3].empty() ) {
            if(getManagerId() )
                ret[pMasqueradingVector[3]]=(Json::UInt64)getValueOfManagerId();
            else
                ret[pMasqueradingVector[3]]=Json::Value();
        }
        return ret;
    }
    LOG_ERROR << "Masquerade failed";
    if(getId() )
        ret["id"]=(Json::UInt64)getValueOfId();
    else
        ret["id"]=Json::Value();
    if(getTitle() )
        ret["title"]=getValueOfTitle();
    else
        ret["title"]=Json::Value();
    if(getDescription() )
        ret["description"]=getValueOfDescription();
    else
        ret["description"]=Json::Value();
    if(getManagerId() )
        ret["manager_id"]=(Json::UInt64)getValueOfManagerId();
    else
        ret["manager_id"]=Json::Value();
    return ret;
}

bool Project::validateJsonForCreation(const Json::Value& pJson, std::string& err)
{
    if(pJson.isMember("id") ) {
        if(!validJsonOfField(0, "id", pJson["id"], err, true) )
            return false;
    }
    if(pJson.isMember("title") ) {
        if(!validJsonOfField(1, "title", pJson["title"], err, true) )
            return false;
    } else{
        err="The title column cannot be null";
        return false;
    }
    if(pJson.isMember("description") ) {
        if(!validJsonOfField(2, "description", pJson["description"], err, true) )
            return false;
    } else{
        err="The description column cannot be null";
        return false;
    }
    if(pJson.isMember("manager_id") ) {
        if(!validJsonOfField(3, "manager_id", pJson["manager_id"], err, true) )
            return false;
    }
    return true;
}

bool Project::validateMasqueradedJsonForCreation(const Json::Value& pJson,
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
            } else{
                err="The " + pMasqueradingVector[1] + " column cannot be null";
                return false;
            }
        }
        if(!pMasqueradingVector[2].empty() ) {
            if(pJson.isMember(pMasqueradingVector[2]) ) {
                if(!validJsonOfField(2, pMasqueradingVector[2], pJson[pMasqueradingVector[2]], err, true) )
                    return false;
            } else{
                err="The " + pMasqueradingVector[2] + " column cannot be null";
                return false;
            }
        }
        if(!pMasqueradingVector[3].empty() ) {
            if(pJson.isMember(pMasqueradingVector[3]) ) {
                if(!validJsonOfField(3, pMasqueradingVector[3], pJson[pMasqueradingVector[3]], err, true) )
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

bool Project::validateJsonForUpdate(const Json::Value& pJson, std::string& err)
{
    if(pJson.isMember("id") ) {
        if(!validJsonOfField(0, "id", pJson["id"], err, false) )
            return false;
    } else{
        err = "The value of primary key must be set in the json object for update";
        return false;
    }
    if(pJson.isMember("title") ) {
        if(!validJsonOfField(1, "title", pJson["title"], err, false) )
            return false;
    }
    if(pJson.isMember("description") ) {
        if(!validJsonOfField(2, "description", pJson["description"], err, false) )
            return false;
    }
    if(pJson.isMember("manager_id") ) {
        if(!validJsonOfField(3, "manager_id", pJson["manager_id"], err, false) )
            return false;
    }
    return true;
}

bool Project::validateMasqueradedJsonForUpdate(const Json::Value& pJson,
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
        } else{
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

bool Project::validJsonOfField(size_t index,
        const std::string& fieldName,
        const Json::Value& pJson,
        std::string& err,
        bool isForCreation)
{
    switch(index) {
    case 0:
        if(isForCreation) {
            err="The automatic primary key cannot be set";
            return false;
        }
        if(pJson.isNull() )
            return true;
        if(!pJson.isUInt64() ) {
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
    case 3:
        if(pJson.isNull() )
            return true;
        if(!pJson.isUInt64() ) {
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

void Project::getTickets(const DbClientPtr& clientPtr,
        const std::function<void(std::vector<Ticket>)>& rcb,
        const ExceptionCallback& ecb) const
{
    const static std::string sql = "select * from ticket where project_id = ?";
    *clientPtr << sql
               << *id_
        >>[rcb = std::move(rcb)](const Result& r){
        std::vector<Ticket> ret;
        ret.reserve(r.size() );
        for(auto const& row : r)
            ret.emplace_back(Ticket(row) );
        rcb(ret);
        }
        >> ecb;
}

void Project::getManager(const DbClientPtr& clientPtr,
        const std::function<void(Account)>& rcb,
        const ExceptionCallback& ecb) const
{
    const static std::string sql = "select * from account where id = ?";
    *clientPtr << sql
               << *managerId_
        >>[rcb = std::move(rcb), ecb](const Result& r){
        if(r.size() == 0)
            ecb(UnexpectedRows("0 rows found") );
        else if(r.size() > 1)
            ecb(UnexpectedRows("Found more than one row") );
        else
            rcb(Account(r[0]) );
        }
        >> ecb;
}

void Project::getStaff(const DbClientPtr& clientPtr,
        const std::function<void(std::vector<std::pair<Account, Staff> >)>& rcb,
        const ExceptionCallback& ecb) const
{
    const static std::string sql =
            "select * from account,staff where staff.project_id = ? and staff.staff_id = account.id";
    *clientPtr << sql
               << *id_
        >>[rcb = std::move(rcb)](const Result& r){
        std::vector<std::pair<Account, Staff> > ret;
        ret.reserve(r.size() );
        for(auto const& row : r)
        {
            ret.emplace_back(std::pair<Account, Staff>(
                            Account(row), Staff(row, Account::getColumnNumber() ) ) );
        }
        rcb(ret);
        }
        >> ecb;
}
