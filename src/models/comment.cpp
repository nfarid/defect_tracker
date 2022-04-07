
#include "comment.hpp"


namespace Model
{


using namespace drogon;
using namespace drogon::orm;


drogon::Task<Comment> Comment::createComment(CoroMapper<Comment>& orm, const
        Util::StringMap& postParams, int32_t userId, int32_t ticketId)
{
    Model::Comment newComment{};
    newComment.setPost(postParams.at("form-post") );
    newComment.setCreatedDate(trantor::Date::now() );
    newComment.setTicketId(ticketId);
    co_return co_await orm.insert(newComment);
}


}  // namespace Model

