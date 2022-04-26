
#ifndef HPP_MODELS_TICKET
#define HPP_MODELS_TICKET

#include "../util/coroutine.hpp"
#include "../util/typedefs.hpp"

#ifdef USE_POSTGRESQL
    #include "generated_postgresql/ticket.hpp"
#else
    #include "generated_sqlite3/ticket.hpp"
#endif  // ifndef USE_POSTGRESQL


namespace Model
{


class Account;
class Comment;
class Project;


/**
 * @brief The Ticket class - an extension of the generated Ticket model with custom methods
 */
class Ticket : public drogon_model::Ticket {
public:
    /**
     * @brief creates a ticket from the data from the form
     * @param orm - the object–relational mapper for Ticket
     * @param formData - the data obtain from the POST request
     * @param fileData - the files obtain from the POST request
     * @param reporterId - the id of the current user
     * @param projectId - the id of the project
     * @return a newly created Ticket if the data is valid
     * @throws Util::FormError if the data is invalid
     */
    static drogon::Task<Ticket> createTicket(const Util::StringMap& formData, const Util::FileMap& fileData,
            PrimaryKeyType reporterId, PrimaryKeyType projectId);

    /**
     * @brief return a list of all the severities as JSON
     */
    static Json::Value getSeverityLst();

    /**
     * @brief return a list of all the statuses as JSON
     */
    static Json::Value getStatusLst();

    /**
     * @brief obtain the statistics of a list of ticket
     */
    static Json::Value getStatistics(const std::vector<Ticket>& ticketLst);

    /**
     * @brief obtain the Ticket model that has the specified primary key
     * @throws drogon::orm::UnexpectedRows if it doesn't exists
     */
    static drogon::Task<Ticket> findByPrimaryKey(PrimaryKeyType ticketId);

    /**
     * @brief delete the Ticket that has the specified primary key
     */
    static drogon::Task<> deleteByPrimaryKey(PrimaryKeyType ticketId);

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
    drogon::Task<bool> canEdit(PrimaryKeyType userId) const;

    /**
     * @brief check if the current user is the reporter of this ticket
     */
    bool isReporter(PrimaryKeyType userId) const;

    /**
     * @brief obtain a list of users that the current user can assign this ticket to
     */
    drogon::Task<std::vector<Account> >getAssignables(PrimaryKeyType userId) const;

    /**
     * @brief update a ticket from the data from the form
     * @param formData - the data obtain from the POST request
     * @param userId - the id of the current user
     */
    drogon::Task<> update(const Util::StringMap& formData, PrimaryKeyType userId);

    /**
     * @brief Turns a model into a Json suitable for the view (uses htmlTranslate)
     */
    Json::Value toViewJson() const;

private:
    using drogon_model::Ticket::Ticket;
    friend drogon::orm::CoroMapper<Ticket>;
};


} // namespace Model

#endif  // ifndef HPP_MODELS_TICKET
