
#include "project.hpp"

#include "account.hpp"
#include "ticket.hpp"

#include "../util/core.hpp"
#include "../util/database.hpp"
#include "../util/form_error.hpp"
#include "../util/string.hpp"

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
using Json::UInt64;


namespace
{


std::vector<std::string_view> splitView(std::string_view str, char delim) {
    std::vector<std::string_view> tokenLst;
    auto beginPos = str.find_first_not_of(delim, 0);
    auto endPos = str.find_first_of(delim, beginPos);
    while(std::string::npos != beginPos || std::string::npos != endPos) {
        tokenLst.push_back(str.substr(beginPos, endPos - beginPos) );
        beginPos = str.find_first_not_of(delim, endPos);
        endPos = str.find_first_of(delim, beginPos);
    }
    return tokenLst;
}


}  // namespace


drogon::Task<Project> Project::createProject(const Util::StringMap& formData, PrimaryKeyType userId)
{
    CoroMapper<Project> projectOrm = Util::getDb();

    // Obtain and trim the data from the POST request
    const std::string title = Util::getTrimmed(formData.at("form-title") );
    const std::string description = Util::getTrimmed(formData.at("form-description") );

    // Validate the data
    if(title.empty() )
        throw Util::FormError("Title cannot be empty");
    if(title.size() > 80)
        throw Util::FormError("Title cannot be longer than 80 characters");
    if(!std::ranges::all_of(title, [](char c){return c == ' ' || c == '-' || Util::isAlNumUnderscore(c);}) )
        throw Util::FormError("Title cannot contain invalid characters");
    if(description.empty() )
        throw Util::FormError("Description cannot be empty");

    // Title must be unique
    const Criteria hasTitle{Model::Project::Cols::_title, CompareOperator::EQ, title};
    if(co_await projectOrm.count(hasTitle) != 0)
        throw Util::FormError("That title already exists");

    Model::Project newProject;
    newProject.setTitle(title);
    newProject.setDescription(description);
    newProject.setManagerId(userId);

    co_return co_await projectOrm.insert(newProject);
}

Task<std::vector<Project> > Project::searchProject(std::string_view getParam) {
    const DbClientPtr db = Util::getDb();

    // Convert the get data from the GET request to a form suitable for tsquery
    // If the getParam is "foo bar <ínválíd;>" then the query is "foo|bar"
    getParam = Util::getTrimmedView(getParam);
    std::string query{};
    for(const std::string_view token : splitView(getParam, ' ') ) {
        const auto trimmedToken = Util::getTrimmedView(token);
        if(Util::isAlNumUnderscore(trimmedToken) )
            query.append(trimmedToken).append("|");
    }
    if(query.empty() ) {
        co_return{};
    }
    query.pop_back();

#ifdef USE_POSTGRESQL
    // Use postgres's full text search to search for projects that match the query
    const Result res = co_await db->execSqlCoro(
        "SELECT * FROM project WHERE to_tsvector(title) @@ to_tsquery($1);",
        query
    );
#else
    query = "%" + query + "%";
    const Result res = co_await db->execSqlCoro(
        "SELECT * FROM project WHERE title LIKE ?",
        query
    );
#endif  // ifdef USE_POSTGRESQL

    // Convert the sql result into a list of project models
    std::vector<Project> projectLst;
    projectLst.reserve(res.size() );
    for(const auto& row : res)
        projectLst.emplace_back(row);

    co_return projectLst;
}

drogon::Task<Project> Project::findByPrimaryKey(PrimaryKeyType projectId) {
    CoroMapper<Project> projectOrm = Util::getDb();
    co_return co_await projectOrm.findByPrimaryKey(projectId);
}

drogon::Task<> Project::deleteByPrimaryKey(PrimaryKeyType projectId) {
    CoroMapper<Project> projectOrm = Util::getDb();
    co_await projectOrm.deleteByPrimaryKey(projectId);
}

Task<Account> Project::getManager() const {
    CoroMapper<Account> accountOrm = Util::getDb();
    co_return co_await accountOrm.findByPrimaryKey(getValueOfManagerId() );
}

Task<std::vector<Ticket> > Project::getTickets() const {
    CoroMapper<Ticket> ticketOrm = Util::getDb();
    const Criteria belongsToProject{Ticket::Cols::_project_id, CompareOperator::EQ, getValueOfId()};
    co_return co_await ticketOrm.findBy(belongsToProject);
}

Task<std::vector<Account> > Project::getStaff() const {
    const DbClientPtr db = Util::getDb();

    // Since user - project is a many-to-many relationship, a staff pivot table is used
#ifdef USE_POSTGRESQL
    const Result res = co_await db->execSqlCoro(
        "SELECT account.* FROM account,staff WHERE staff.project_id = $1 AND staff.staff_id = account.id;",
        getValueOfId()
    );
#else
    const Result res = co_await db->execSqlCoro(
        "SELECT account.* FROM account,staff WHERE staff.project_id = ? AND staff.staff_id = account.id;",
        getValueOfId()
    );
#endif  // ifdef USE_POSTGRESQL

    // Convert the sql result into a list of user models
    std::vector<Account> staffLst;
    staffLst.reserve(res.size()+1);
    staffLst.push_back(co_await getManager() );
    for(const auto& row : res)
        staffLst.emplace_back(row);

    co_return staffLst;
}

Task<bool> Project::isStaff(PrimaryKeyType userId) const {
    const std::vector staffLst = co_await getStaff();
    for(const auto& staff : staffLst) {
        if(staff.getValueOfId() == userId)
            co_return true;
    }
    co_return false;
}

bool Project::isManager(PrimaryKeyType userId) const {
    return userId == getValueOfManagerId();
}

Json::Value Project::toViewJson() const {
    Json::Value json{};
    json["id"] = static_cast<UInt64>(getValueOfId() );
    json["title"] = HttpViewData::htmlTranslate(getValueOfTitle() );
    json["description"] = HttpViewData::htmlTranslate(getValueOfDescription() );
    json["manager-id"] = static_cast<UInt64>(getValueOfManagerId() );
    return json;
}


}  // namespace Model

