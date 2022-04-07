
#include "../util/coroutine.hpp"
#include "../util/typedefs.hpp"
#include "generated/comment.hpp"


namespace Model
{


class Comment : public drogon_model::bug_tracker::Comment {
public:
    static drogon::Task<Comment> createComment(drogon::orm::CoroMapper<Comment>& orm,
            const Util::StringMap& postParams,
            int32_t userId, int32_t ticketId);

    using drogon_model::bug_tracker::Comment::Comment;

    friend drogon::orm::CoroMapper<Comment>;
};


} // namespace Model

