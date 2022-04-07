
#include "ticket.hpp"

#include "account.hpp"
#include "comment.hpp"
#include "project.hpp"
#include "../util/core.hpp"

#include <vector>
#include <string>
#include <string_view>


namespace Model
{


using namespace drogon;
using namespace drogon::orm;


UTIL_INTERNAL std::vector<std::string> severityLst = {"low", "medium", "high"};
UTIL_INTERNAL std::vector<std::string> statusLst = {"new", "confirmed", "unreproducible", "resolved", "reopened"};

UTIL_INTERNAL Json::Value lstToJson(const std::vector<std::string>& lst) {
    Json::Value json{};
    for(const auto& elem : lst)
        json.append(elem);
    return json;
}

drogon::Task<Ticket> Ticket::createTicket(drogon::orm::CoroMapper<Ticket>& orm,
        const std::unordered_map<std::string, std::string>& postParams, int32_t reporterId, int32_t projectId)
{
    const std::string title = postParams.at("form-title");
    const std::string description = postParams.at("form-description");
    const std::string severity = postParams.at("form-severity");

    if(std::find(begin(severityLst), end(severityLst), severity) == end(severityLst) )
        throw std::runtime_error("Form Error: Invalid severity");

    Ticket newTicket;
    newTicket.setTitle(title);
    newTicket.setDescription(description);
    newTicket.setSeverity(severity);
    newTicket.setStatus("new");
    newTicket.setCreatedDate(trantor::Date::now() );
    newTicket.setReporterId(reporterId);
    newTicket.setProjectId(projectId);

    co_return co_await orm.insert(newTicket);
}

Json::Value Ticket::getSeverityLst() {
    // Note: initialising static variables is thread safe
    const static Json::Value severityLstJson = lstToJson(severityLst);
    return severityLstJson;
}

Json::Value Ticket::getStatusLst() {
    // Note: initialising static variables is thread safe
    const static Json::Value statusLstJson = lstToJson(statusLst);
    return statusLstJson;
}

Task<Account> Ticket::getReporter(DbClientPtr db) const {
    const static std::string query = "SELECT * FROM Account WHERE id = $1";
    const int32_t reporterId = getValueOfReporterId();
    const Result res = co_await db->execSqlCoro(query, reporterId);
    if(res.empty() )
        throw UnexpectedRows("0 rows found");
    if(res.size() > 1)
        throw UnexpectedRows("Found more than one row");
    co_return Account{res.front()};
}

drogon::Task<Project> Ticket::getProject(DbClientPtr db) const {
    const static std::string query = "SELECT * FROM Project WHERE id = $1";
    const int32_t projectId = getValueOfProjectId();
    const Result res = co_await db->execSqlCoro(query, projectId);
    if(res.empty() )
        throw UnexpectedRows("0 rows found");
    if(res.size() > 1)
        throw UnexpectedRows("Found more than one row");
    co_return Project{res.front()};
}

drogon::Task<std::vector<Comment> > Ticket::getComments(DbClientPtr db) const {
    const static std::string query = "SELECT * FROM Comment WHERE poster_id = $1";
    const int32_t id = getValueOfId();
    const Result res = co_await db->execSqlCoro(query, id);
    std::vector<Comment> commentLst;
    commentLst.reserve(res.size() );
    for(const auto& row : res)
        commentLst.emplace_back(row);
    co_return commentLst;
}

drogon::Task<bool> Ticket::canEdit(DbClientPtr db, int32_t userId) const {
    if(isReporter(userId) )
        co_return true;

    const Model::Project project = co_await getProject(db);
    co_return co_await project.isStaff(db, userId);
}

bool Ticket::isReporter(int32_t userId) const {
    return userId == getValueOfReporterId();
}

drogon::Task<std::vector<Account> > Ticket::getAssignables(DbClientPtr db, int32_t userId) const {
    const Model::Project project = co_await getProject(db);

    // Manager can assign to any staff
    if(project.getValueOfManagerId() == userId)
        co_return co_await project.getStaff(db);

    // If the ticket is not yet assigned, then a staff can self-assign
    if(!getAssignedId() && co_await project.isStaff(db, userId) ) {
        CoroMapper<Account> accountOrm{db};
        const Account staff = co_await accountOrm.findByPrimaryKey(userId);
        co_return{staff};
    }

    // Else the user cannot assign the ticket
    co_return{};
}


}  // namespace Model

