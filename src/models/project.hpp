
#ifndef HPP_MODELS_PROJECT
#define HPP_MODELS_PROJECT

#include "../util/coroutine.hpp"
#include "../util/typedefs.hpp"

#ifdef USE_POSTGRESQL
    #include "generated_postgresql/project.hpp"
#else
    #include "generated_sqlite3/project.hpp"
#endif  // ifndef USE_POSTGRESQL



namespace Model
{


class Account;
class Ticket;

/**
 * @brief The Project class - an extension of the generated Project model with custom methods
 */
class Project : public drogon_model::Project {
public:
    /**
     * @brief validates the data from the POST parameter
     * @param orm - the object–relational mapper for Project
     * @param postParams - the data obtain from the POST request
     * @param userId - the id of the current user
     * @return a newly created Project if the data is valid
     * @throws Util::FormError if the data is invalid
     */
    static drogon::Task<Project> createProject(const Util::StringMap& postParams, PrimaryKeyType userId);

    /**
     * @brief search for projects that match the query
     * @param db - the database client
     * @param getParam - the query from a GET request
     * @return a list of projects
     */
    static drogon::Task<std::vector<Project> > searchProject(std::string_view getParam);

    /**
     * @brief obtain the manager of this project
     */
    drogon::Task<Account> getManager() const;

    /**
     * @brief obtain all the tickets that this project has
     */
    drogon::Task<std::vector<Ticket> > getTickets() const;

    /**
     * @brief obtain a list of staff that are working on thus project
     */
    drogon::Task<std::vector<Account> > getStaff() const;

    /**
     * @brief check if the user with the specified userId is a staff of this project
     */
    drogon::Task<bool> isStaff(PrimaryKeyType userId) const;

    /**
     * @brief check if the user with the specified userId is the manager of this project
     */
    bool isManager(PrimaryKeyType userId) const;

    /**
     * @brief Turns a model into a Json suitable for the view (uses htmlTranslate)
     */
    Json::Value toViewJson() const;

private:
    using drogon_model::Project::Project;

    friend drogon::orm::CoroMapper<Project>;
};


} // namespace Model

#endif  // ifndef HPP_MODELS_PROJECT
