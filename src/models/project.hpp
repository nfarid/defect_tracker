
// This file is generated by drogon_ctl

#pragma once
#include <drogon/orm/Result.h>
#include <drogon/orm/Row.h>
#include <drogon/orm/Field.h>
#include <drogon/orm/SqlBinder.h>
#include <drogon/orm/Mapper.h>
#ifdef __cpp_impl_coroutine
#include <drogon/orm/CoroMapper.h>
#endif  // ifdef __cpp_impl_coroutine
#include <trantor/utils/Date.h>
#include <trantor/utils/Logger.h>
#include <json/json.h>
#include <string>
#include <memory>
#include <vector>
#include <tuple>
#include <stdint.h>
#include <iostream>

using namespace drogon::orm;


namespace drogon
{


namespace orm
{


class DbClient;
using DbClientPtr = std::shared_ptr<DbClient>;


}  // namespace orm


}  // namespace drogon


namespace drogon_model
{


namespace bug_tracker
{


class Project{
public:
    struct Cols{
        static const std::string _id;
        static const std::string _project_name;
        static const std::string _manager_id;
    };

    const static int primaryKeyNumber;
    const static std::string tableName;
    const static bool hasPrimaryKey;
    const static std::string primaryKeyName;
    using PrimaryKeyType = int32_t;
    const PrimaryKeyType& getPrimaryKey() const;

    /**
     * @brief constructor
     * @param r One row of records in the SQL query result.
     * @param indexOffset Set the offset to -1 to access all columns by column names,
     * otherwise access all columns by offsets.
     * @note If the SQL is not a style of 'select * from table_name ...' (select all
     * columns by an asterisk), please set the offset to -1.
     */
    explicit Project(const Row& r, const ssize_t indexOffset = 0) noexcept;

    /**
     * @brief constructor
     * @param pJson The json object to construct a new instance.
     */
    explicit Project(const Json::Value& pJson) noexcept(false);

    /**
     * @brief constructor
     * @param pJson The json object to construct a new instance.
     * @param pMasqueradingVector The aliases of table columns.
     */
    Project(const Json::Value& pJson, const std::vector<std::string>& pMasqueradingVector) noexcept(false);

    Project() = default;

    void updateByJson(const Json::Value& pJson) noexcept(false);
    void updateByMasqueradedJson(const Json::Value& pJson,
            const std::vector<std::string>& pMasqueradingVector) noexcept(false);
    static bool validateJsonForCreation(const Json::Value& pJson, std::string& err);
    static bool validateMasqueradedJsonForCreation(const Json::Value&,
            const std::vector<std::string>& pMasqueradingVector,
            std::string& err);
    static bool validateJsonForUpdate(const Json::Value& pJson, std::string& err);
    static bool validateMasqueradedJsonForUpdate(const Json::Value&,
            const std::vector<std::string>& pMasqueradingVector,
            std::string& err);
    static bool validJsonOfField(size_t index,
            const std::string& fieldName,
            const Json::Value& pJson,
            std::string& err,
            bool isForCreation);

    /**  For column id  */
    ///Get the value of the column id, returns the default value if the column is null
    const int32_t& getValueOfId() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<int32_t>& getId() const noexcept;

    ///Set the value of the column id
    void setId(const int32_t& pId) noexcept;


    /**  For column project_name  */
    ///Get the value of the column project_name, returns the default value if the column is null
    const std::string& getValueOfProjectName() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string>& getProjectName() const noexcept;

    ///Set the value of the column project_name
    void setProjectName(const std::string& pProjectName) noexcept;
    void setProjectName(std::string&& pProjectName) noexcept;


    /**  For column manager_id  */
    ///Get the value of the column manager_id, returns the default value if the column is null
    const int32_t& getValueOfManagerId() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<int32_t>& getManagerId() const noexcept;

    ///Set the value of the column manager_id
    void setManagerId(const int32_t& pManagerId) noexcept;
    void setManagerIdToNull() noexcept;



    static size_t getColumnNumber() noexcept {
        return 3;
    }

    static const std::string& getColumnName(size_t index) noexcept(false);

    Json::Value toJson() const;
    Json::Value toMasqueradedJson(const std::vector<std::string>& pMasqueradingVector) const;
    /// Relationship interfaces

private:
    friend Mapper<Project>;
#ifdef __cpp_impl_coroutine
    friend CoroMapper<Project>;
#endif  // ifdef __cpp_impl_coroutine
    static const std::vector<std::string>& insertColumns() noexcept;
    void outputArgs(drogon::orm::internal::SqlBinder& binder) const;
    const std::vector<std::string> updateColumns() const;
    void updateArgs(drogon::orm::internal::SqlBinder& binder) const;
    ///For mysql or sqlite3
    void updateId(const uint64_t id);
    std::shared_ptr<int32_t> id_;
    std::shared_ptr<std::string> projectName_;
    std::shared_ptr<int32_t> managerId_;
    struct MetaData{
        const std::string colName_;
        const std::string colType_;
        const std::string colDatabaseType_;
        const ssize_t colLength_;
        const bool isAutoVal_;
        const bool isPrimaryKey_;
        const bool notNull_;
    };
    static const std::vector<MetaData> metaData_;
    bool dirtyFlag_[3]={ false};

public:
    static const std::string& sqlForFindingByPrimaryKey()
    {
        static const std::string sql="select * from " + tableName + " where id = $1";
        return sql;
    }

    static const std::string& sqlForDeletingByPrimaryKey()
    {
        static const std::string sql="delete from " + tableName + " where id = $1";
        return sql;
    }

    std::string sqlForInserting(bool& needSelection) const
    {
        std::string sql="insert into " + tableName + " (";
        size_t parametersCount = 0;
        needSelection = false;
        sql += "id,";
        ++parametersCount;
        if(dirtyFlag_[1]) {
            sql += "project_name,";
            ++parametersCount;
        }
        if(dirtyFlag_[2]) {
            sql += "manager_id,";
            ++parametersCount;
        }
        needSelection=true;
        if(parametersCount > 0) {
            sql[sql.length()-1]=')';
            sql += " values (";
        } else {
            sql += ") values (";
        }

        int placeholder=1;
        char placeholderStr[64];
        size_t n=0;
        sql +="default,";
        if(dirtyFlag_[1]) {
            n = static_cast<size_t>(sprintf(placeholderStr, "$%d,", placeholder++) );
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[2]) {
            n = static_cast<size_t>(sprintf(placeholderStr, "$%d,", placeholder++) );
            sql.append(placeholderStr, n);
        }
        if(parametersCount > 0)
            sql.resize(sql.length() - 1);
        if(needSelection)
            sql.append(") returning *");
        else
            sql.append(1, ')');
        LOG_TRACE << sql;
        return sql;
    }
};


}  // namespace bug_tracker


} // namespace drogon_model


namespace Model = drogon_model::bug_tracker;
