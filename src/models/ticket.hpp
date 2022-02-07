
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


class Ticket{
public:
    struct Cols{
        static const std::string _id;
        static const std::string _title;
        static const std::string _descr;
        static const std::string _project_module;
        static const std::string _ticket_status;
        static const std::string _severity;
        static const std::string _assigned;
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
    explicit Ticket(const Row& r, const ssize_t indexOffset = 0) noexcept;

    /**
     * @brief constructor
     * @param pJson The json object to construct a new instance.
     */
    explicit Ticket(const Json::Value& pJson) noexcept(false);

    /**
     * @brief constructor
     * @param pJson The json object to construct a new instance.
     * @param pMasqueradingVector The aliases of table columns.
     */
    Ticket(const Json::Value& pJson, const std::vector<std::string>& pMasqueradingVector) noexcept(false);

    Ticket() = default;

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


    /**  For column title  */
    ///Get the value of the column title, returns the default value if the column is null
    const std::string& getValueOfTitle() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string>& getTitle() const noexcept;

    ///Set the value of the column title
    void setTitle(const std::string& pTitle) noexcept;
    void setTitle(std::string&& pTitle) noexcept;


    /**  For column descr  */
    ///Get the value of the column descr, returns the default value if the column is null
    const std::string& getValueOfDescr() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string>& getDescr() const noexcept;

    ///Set the value of the column descr
    void setDescr(const std::string& pDescr) noexcept;
    void setDescr(std::string&& pDescr) noexcept;


    /**  For column project_module  */
    ///Get the value of the column project_module, returns the default value if the column is null
    const std::string& getValueOfProjectModule() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string>& getProjectModule() const noexcept;

    ///Set the value of the column project_module
    void setProjectModule(const std::string& pProjectModule) noexcept;
    void setProjectModule(std::string&& pProjectModule) noexcept;
    void setProjectModuleToNull() noexcept;


    /**  For column ticket_status  */
    ///Get the value of the column ticket_status, returns the default value if the column is null
    const std::string& getValueOfTicketStatus() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string>& getTicketStatus() const noexcept;

    ///Set the value of the column ticket_status
    void setTicketStatus(const std::string& pTicketStatus) noexcept;
    void setTicketStatus(std::string&& pTicketStatus) noexcept;


    /**  For column severity  */
    ///Get the value of the column severity, returns the default value if the column is null
    const std::string& getValueOfSeverity() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string>& getSeverity() const noexcept;

    ///Set the value of the column severity
    void setSeverity(const std::string& pSeverity) noexcept;
    void setSeverity(std::string&& pSeverity) noexcept;


    /**  For column assigned  */
    ///Get the value of the column assigned, returns the default value if the column is null
    const int32_t& getValueOfAssigned() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<int32_t>& getAssigned() const noexcept;

    ///Set the value of the column assigned
    void setAssigned(const int32_t& pAssigned) noexcept;
    void setAssignedToNull() noexcept;



    static size_t getColumnNumber() noexcept {
        return 7;
    }

    static const std::string& getColumnName(size_t index) noexcept(false);

    Json::Value toJson() const;
    Json::Value toMasqueradedJson(const std::vector<std::string>& pMasqueradingVector) const;
    /// Relationship interfaces

private:
    friend Mapper<Ticket>;
#ifdef __cpp_impl_coroutine
    friend CoroMapper<Ticket>;
#endif  // ifdef __cpp_impl_coroutine
    static const std::vector<std::string>& insertColumns() noexcept;
    void outputArgs(drogon::orm::internal::SqlBinder& binder) const;
    const std::vector<std::string> updateColumns() const;
    void updateArgs(drogon::orm::internal::SqlBinder& binder) const;
    ///For mysql or sqlite3
    void updateId(const uint64_t id);
    std::shared_ptr<int32_t> id_;
    std::shared_ptr<std::string> title_;
    std::shared_ptr<std::string> descr_;
    std::shared_ptr<std::string> projectModule_;
    std::shared_ptr<std::string> ticketStatus_;
    std::shared_ptr<std::string> severity_;
    std::shared_ptr<int32_t> assigned_;
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
    bool dirtyFlag_[7]={ false};

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
            sql += "title,";
            ++parametersCount;
        }
        if(dirtyFlag_[2]) {
            sql += "descr,";
            ++parametersCount;
        }
        if(dirtyFlag_[3]) {
            sql += "project_module,";
            ++parametersCount;
        }
        if(dirtyFlag_[4]) {
            sql += "ticket_status,";
            ++parametersCount;
        }
        if(dirtyFlag_[5]) {
            sql += "severity,";
            ++parametersCount;
        }
        if(dirtyFlag_[6]) {
            sql += "assigned,";
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
            n = sprintf(placeholderStr, "$%d,", placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[2]) {
            n = sprintf(placeholderStr, "$%d,", placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[3]) {
            n = sprintf(placeholderStr, "$%d,", placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[4]) {
            n = sprintf(placeholderStr, "$%d,", placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[5]) {
            n = sprintf(placeholderStr, "$%d,", placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[6]) {
            n = sprintf(placeholderStr, "$%d,", placeholder++);
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
