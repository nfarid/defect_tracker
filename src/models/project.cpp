
#include "project.hpp"

#include "account.hpp"
#include "ticket.hpp"

#include "../util/core.hpp"
#include "../util/form_error.hpp"

#include <cctype>
#include <string>
#include <string_view>
#include <vector>


namespace Model
{


using namespace drogon;
using namespace drogon::orm;


UTIL_INTERNAL std::vector<std::string> split(std::string_view str, std::string_view delim) {
    std::vector<std::string> tokens;
    auto beginPos = str.find_first_not_of(delim, 0);
    auto endPos = str.find_first_of(delim, beginPos);
    while(std::string::npos != beginPos || std::string::npos != endPos) {
        tokens.emplace_back(str.substr(beginPos, endPos - beginPos) );
        beginPos = str.find_first_not_of(delim, endPos);
        endPos = str.find_first_of(delim, beginPos);
    }
    return tokens;
}

UTIL_INTERNAL bool isalnum(std::string_view str) {
    for(const char c : str) {
        if(!std::isalnum(c) )
            return false;
    }
    return true;
}

drogon::Task<Project> Project::createProject(drogon::orm::CoroMapper<Project>& orm,
        const std::unordered_map<std::string, std::string>& postParams, int32_t userId)
{
    const std::string& title = postParams.at("form-title");
    const std::string& description = postParams.at("form-description");

    // TODO: Add more requirements for a valid title & description
    if(title.empty() )
        throw Util::FormError("Title cannot be empty");
    if(description.empty() )
        throw Util::FormError("Description cannot be empty");

    // Title must be unique
    const Criteria hasTitle{Model::Project::Cols::_title, CompareOperator::EQ, title};
    if(co_await orm.count(hasTitle) != 0)
        throw Util::FormError("That title already exists");

    Model::Project newProject;
    newProject.setTitle(title);
    newProject.setDescription(description);
    newProject.setManagerId(userId);

    co_return co_await orm.insert(newProject);
}

Task<std::vector<Project> > Project::searchProject(DbClientPtr db, std::string_view urlQuery) {
    std::string tsQuery{};
    for(const auto& token : split(urlQuery, " ") ) {
        if(isalnum(token) )
            tsQuery.append(token).append("|");
    }
    if(tsQuery.empty() ) {
        co_return{};
    }
    tsQuery.pop_back();

    const Result res = co_await db->execSqlCoro(
        "SELECT * FROM project WHERE to_tsvector(title) @@ to_tsquery($1)", tsQuery
    );
    std::vector<Project> projectLst;
    projectLst.reserve(res.size() );
    for(const auto& row : res)
        projectLst.emplace_back(row);

    co_return projectLst;
}

Task<Account> Project::getManager(DbClientPtr db) const {
    const static std::string query = "SELECT * FROM Account WHERE id = $1";
    const int32_t managerId = getValueOfManagerId();
    const Result res = co_await db->execSqlCoro(query, managerId);
    if(res.empty() )
        throw UnexpectedRows("0 rows found");
    if(res.size() > 1)
        throw UnexpectedRows("Found more than one row");
    co_return Account(res[0]);
}

Task<std::vector<Ticket> > Project::getTickets(DbClientPtr db) const {
    const static std::string query = "SELECT * FROM Ticket WHERE project_id = $1";
    const int32_t id = getValueOfId();
    const Result res = co_await db->execSqlCoro(query, id);
    std::vector<Ticket> ticketLst;
    ticketLst.reserve(res.size() );
    for(const auto& row : res)
        ticketLst.emplace_back(row);
    co_return ticketLst;
}

Task<std::vector<Account> > Project::getStaff(DbClientPtr db) const {
    const static std::string sql =
            "SELECT * FROM Account,Staff WHERE Staff.project_id = $1 AND Staff.staff_id = Account.id";
    const int32_t id = getValueOfId();
    const Result res = co_await db->execSqlCoro(sql, id);
    std::vector<Account> staffLst;
    staffLst.reserve(res.size()+1);
    for(const auto& row : res)
        staffLst.emplace_back(row);
    staffLst.push_back(co_await getManager(db) );
    co_return staffLst;
}

Task<bool> Project::isStaff(DbClientPtr db, int32_t userId) const {
    const std::vector staffLst = co_await getStaff(db);
    for(const auto& staff : staffLst) {
        if(staff.getValueOfId() == userId)
            co_return true;
    }
    co_return false;
}


}  // namespace Model

