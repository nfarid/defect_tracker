
#ifndef HPP_MODELS_PROJECT
#define HPP_MODELS_PROJECT

#include "../util/coroutine.hpp"
#include "../util/typedefs.hpp"
#include "generated/project.hpp"

#include <drogon/utils/coroutine.h>


namespace Model
{


class Account;
class Ticket;

/**
 * @brief The Project class - an extension of the generated Project model with custom methods
 */
class Project : public drogon_model::bug_tracker::Project {
public:
    /**
     * @brief validates the data from the POST parameter
     * @param orm - the object–relational mapper for Project
     * @param postParams - the data obtain from the POST request
     * @param userId - the id of the current user
     * @return a newly created Project if the data is valid
     * @throws Util::FormError if the data is invalid
     */
    static drogon::Task<Project> createProject(drogon::orm::CoroMapper<Project>& orm,
            const std::unordered_map<std::string, std::string>& postParams, int32_t userId);

    /**
     * @brief search for projects that match the query
     * @param db - the database client
     * @param urlQuery - the query from a GET request
     * @return a list of projects
     */
    static drogon::Task<std::vector<Project> > searchProject(drogon::orm::DbClientPtr db, std::string_view urlQuery);

    /**
     * @brief obtain the manager of this project
     */
    drogon::Task<Account> getManager(drogon::orm::DbClientPtr db) const;

    /**
     * @brief obtain all the tickets that this project has
     */
    drogon::Task<std::vector<Ticket> > getTickets(drogon::orm::DbClientPtr db) const;

    /**
     * @brief obtain a list of staff that are working on thus project
     */
    drogon::Task<std::vector<Account> > getStaff(drogon::orm::DbClientPtr db) const;

    /**
     * @brief check if the user with the specified userId is a staff of this project
     */
    drogon::Task<bool> isStaff(drogon::orm::DbClientPtr db, int32_t userId) const;


private:
    using drogon_model::bug_tracker::Project::Project;

    friend drogon::orm::CoroMapper<Project>;
};


} // namespace Model

#endif // ifndef HPP_MODELS_PROJECT
