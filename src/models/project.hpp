
#include "../util/coroutine.hpp"
#include "generated/project.hpp"

#include <drogon/utils/coroutine.h>


namespace Model
{


class Account;
class Ticket;

class Project : public drogon_model::bug_tracker::Project {
public:
    using drogon_model::bug_tracker::Project::Project;

    friend drogon::orm::CoroMapper<Project>;
    static drogon::Task<Project> createProject(drogon::orm::CoroMapper<Project>& orm,
            const std::unordered_map<std::string, std::string>& postParams, int32_t userId);
    static drogon::Task<std::vector<Project> > searchProject(drogon::orm::DbClientPtr db, std::string_view urlQuery);
    drogon::Task<Account> getManager(drogon::orm::DbClientPtr db) const;
    drogon::Task<std::vector<Ticket> > getTickets(drogon::orm::DbClientPtr db) const;
    drogon::Task<std::vector<Account> > getStaff(drogon::orm::DbClientPtr db) const;
    drogon::Task<bool> isStaff(drogon::orm::DbClientPtr db, int32_t userId) const;
};


} // namespace Model

