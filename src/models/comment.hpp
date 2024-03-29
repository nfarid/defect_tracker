
#ifndef HPP_MODELS_COMMENT
#define HPP_MODELS_COMMENT

#include "../util/coroutine.hpp"
#include "../util/typedefs.hpp"

#ifdef USE_POSTGRESQL
    #include "generated_postgresql/comment.hpp"
#else
    #include "generated_sqlite3/comment.hpp"
#endif  // ifndef USE_POSTGRESQL


namespace Model
{


/**
 * @brief The Comment class - an extension of the generated Comment model with custom methods
 */
class Comment : public drogon_model::Comment {
public:

    /**
     * @brief validates the data from the comment creation form
     * @param orm - the object–relational mapper for Comment
     * @param formData - the data obtain from the POST request
     * @param userId - the id of the current user
     * @param ticketId - the id of the ticket for this comment's form
     * @return a newly created Comment if the data is valid
     * @throws Util::FormError if the data is invalid
     */
    static drogon::Task<Comment> createComment(const Util::StringMap& formData, PrimaryKeyType userId,
            PrimaryKeyType ticketId);

    /**
     * @brief Turns a model into a Json suitable for the view (uses htmlTranslate)
     */
    Json::Value toViewJson() const;

private:
    using drogon_model::Comment::Comment;

    friend drogon::orm::CoroMapper<Comment>;
};


} // namespace Model

#endif  // ifndef HPP_MODELS_COMMENT
