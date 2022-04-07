
#include "../util/coroutine.hpp"
#include "generated/ticket.hpp"


namespace Model
{


class Account;
class Comment;
class Project;


class Ticket : public drogon_model::bug_tracker::Ticket {
public:
    using drogon_model::bug_tracker::Ticket::Ticket;
    friend drogon::orm::CoroMapper<Ticket>;

    static drogon::Task<Ticket> createTicket(drogon::orm::CoroMapper<Ticket>& orm,
            const std::unordered_map<std::string, std::string>& postParams, int32_t reporterId, int32_t projectId);

    static Json::Value getSeverityLst();
    static Json::Value getStatusLst();

    drogon::Task<Account> getReporter(drogon::orm::DbClientPtr db) const;
    drogon::Task<Project> getProject(drogon::orm::DbClientPtr db) const;
    drogon::Task<std::vector<Comment> > getComments(drogon::orm::DbClientPtr db) const;

    drogon::Task<bool> canEdit(drogon::orm::DbClientPtr db, int32_t userId) const;
    bool isReporter(int32_t userId) const;
    drogon::Task<std::vector<Account> > getAssignables(drogon::orm::DbClientPtr db, int32_t userId) const;
};


} // namespace Model

