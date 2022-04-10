
#ifndef HPP_MODELS_TICKET
#define HPP_MODELS_TICKET

#include "../util/coroutine.hpp"
#include "../util/typedefs.hpp"
#include "generated/ticket.hpp"


namespace Model
{


class Account;
class Comment;
class Project;


/**
 * @brief The Ticket class - an extension of the generated Ticket model with custom methods
 */
class Ticket : public drogon_model::bug_tracker::Ticket {
public:
    /**
     * @brief creates a ticket from the data from the form
     * @param orm - the object–relational mapper for Ticket
     * @param postParams - the data obtain from the POST request
     * @param reporterId - the id of the current user
     * @param projectId - the id of the project
     * @return a newly created Ticket if the data is valid
     * @throws Util::FormError if the data is invalid
     */
    static drogon::Task<Ticket> createTicket(const Util::StringMap& postParams, int32_t reporterId, int32_t projectId);

    /**
     * @brief return a list of all the severities as JSON
     */
    static Json::Value getSeverityLst();

    /**
     * @brief return a list of all the statuses as JSON
     */
    static Json::Value getStatusLst();

    /**
     * @brief obtain the reporter of this project
     */
    drogon::Task<Account> getReporter() const;

    /**
     * @brief obtain the project of that this ticket belongs to
     */
    drogon::Task<Project> getProject() const;

    /**
     * @brief obtain a list of comments that this ticket has
     */
    drogon::Task<std::vector<Comment> > getComments() const;

    /**
     * @brief check if the current user can edit this ticket
     */
    drogon::Task<bool> canEdit(int32_t userId) const;

    /**
     * @brief check if the current user is the reporter of this ticket
     */
    bool isReporter(int32_t userId) const;

    /**
     * @brief obtain a list of users that the current user can assign this ticket to
     */
    drogon::Task<std::vector<Account> >getAssignables(int32_t userId) const;

    /**
     * @brief update a ticket from the data from the form
     * @param postParams - the data obtain from the POST request
     * @param userId - the id of the current user
     */
    drogon::Task<> update(const Util::StringMap& postParams, int32_t userId);

    /**
     * @brief Turns a model into a Json suitable for the view (uses htmlTranslate)
     */
    Json::Value toViewJson() const;

private:
    using drogon_model::bug_tracker::Ticket::Ticket;
    friend drogon::orm::CoroMapper<Ticket>;
};


} // namespace Model

#endif  // ifndef HPP_MODELS_TICKET
