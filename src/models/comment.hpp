
#ifndef HPP_MODELS_COMMENT
#define HPP_MODELS_COMMENT

#include "../util/coroutine.hpp"
#include "../util/typedefs.hpp"
#include "generated/comment.hpp"


namespace Model
{


/**
 * @brief The Comment class - an extension of the generated Comment model with custom methods
 */
class Comment : public drogon_model::bug_tracker::Comment {
public:
    /**
     * @brief validates the data from the comment creation form
     * @param orm - the object–relational mapper for Comment
     * @param postParams - the data obtain from the POST request
     * @param userId - the id of the current user
     * @param ticketId - the id of the ticket for this comment's form
     * @return a newly created Comment if the data is valid
     * @throws Util::FormError if the data is invalid
     */
    static drogon::Task<Comment> createComment(drogon::orm::CoroMapper<Comment>& orm,
            const Util::StringMap& postParams,
            int32_t userId, int32_t ticketId);


private:
    using drogon_model::bug_tracker::Comment::Comment;

    friend drogon::orm::CoroMapper<Comment>;
};


} // namespace Model

#endif // ifndef HPP_MODELS_COMMENT
