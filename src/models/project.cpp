
#include "project.hpp"

#include "account.hpp"
#include "ticket.hpp"
#include "../util/core.hpp"
#include "../util/form_error.hpp"

#include <drogon/HttpAppFramework.h>
#include <drogon/HttpViewData.h>

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

drogon::Task<Project> Project::createProject(const Util::StringMap& postParams, int32_t userId)
{
    CoroMapper<Project> orm = app().getDbClient("db");

    // Obtain the data from the POST request
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

Task<std::vector<Project> > Project::searchProject(std::string_view getQuery) {
    const DbClientPtr db = app().getDbClient("db");

    // Convert the query from the GET request to a form suitable for tsquery
    std::string tsQuery{};
    for(const auto& token : split(getQuery, " ") ) {
        if(isalnum(token) )
            tsQuery.append(token).append("|");
    }
    if(tsQuery.empty() ) {
        co_return{};
    }
    tsQuery.pop_back();

    // Use postgres's full text search to search for projects that match the query
    const Result res = co_await db->execSqlCoro(
        "SELECT * FROM project WHERE to_tsvector(title) @@ to_tsquery($1)", tsQuery
    );

    // Convert the sql result into a list of project models
    std::vector<Project> projectLst;
    projectLst.reserve(res.size() );
    for(const auto& row : res)
        projectLst.emplace_back(row);

    co_return projectLst;
}

Task<Account> Project::getManager() const {
    CoroMapper<Account> accountOrm = app().getDbClient("db");
    co_return co_await accountOrm.findByPrimaryKey(getValueOfManagerId() );
}

Task<std::vector<Ticket> > Project::getTickets() const {
    CoroMapper<Ticket> ticketOrm = app().getDbClient("db");
    const Criteria belongsToProject{Ticket::Cols::_project_id, CompareOperator::EQ, getValueOfId()};
    co_return co_await ticketOrm.findBy(belongsToProject);
}

Task<std::vector<Account> > Project::getStaff() const {
    const DbClientPtr db = app().getDbClient("db");

    // Since user - project is a many-to-many relationship, a staff pivot table is used
    const static std::string sql =
            "SELECT Account.* FROM Account,Staff WHERE Staff.project_id = $1 AND Staff.staff_id = Account.id";
    const int32_t id = getValueOfId();
    const Result res = co_await db->execSqlCoro(sql, id);

    // Convert the sql result into a list of user models
    std::vector<Account> staffLst;
    staffLst.reserve(res.size()+1);
    staffLst.push_back(co_await getManager() );
    for(const auto& row : res)
        staffLst.emplace_back(row);

    co_return staffLst;
}

Task<bool> Project::isStaff(int32_t userId) const {
    const std::vector staffLst = co_await getStaff();
    for(const auto& staff : staffLst) {
        if(staff.getValueOfId() == userId)
            co_return true;
    }
    co_return false;
}

Json::Value Project::toViewJson() const {
    Json::Value json{};
    json["id"] = getValueOfId();
    json["title"] = HttpViewData::htmlTranslate(getValueOfTitle() );
    json["description"] = HttpViewData::htmlTranslate(getValueOfDescription() );
    json["manager-id"] = getValueOfManagerId();
    return json;
}


}  // namespace Model

