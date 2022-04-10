
#include "comment.hpp"

#include "../util/constants.hpp"

#include <drogon/HttpAppFramework.h>
#include <drogon/HttpViewData.h>


namespace Model
{


using namespace drogon;
using namespace drogon::orm;


drogon::Task<Comment> Comment::createComment(const Util::StringMap& postParams, int32_t userId, int32_t ticketId)
{
    CoroMapper<Comment> orm = app().getDbClient("db");

    Model::Comment newComment{};
    newComment.setPost(postParams.at("form-post") );
    newComment.setCreatedDate(trantor::Date::now() );
    newComment.setPosterId(userId);
    newComment.setTicketId(ticketId);
    co_return co_await orm.insert(newComment);
}

Json::Value Comment::toViewJson() const {
    Json::Value json{};
    json["id"] = getValueOfId();
    json["post"] = HttpViewData::htmlTranslate(getValueOfPost() );
    const auto createdDate = getValueOfCreatedDate().toCustomedFormattedString(Util::dateFormat);
    json["created-date"] = HttpViewData::htmlTranslate(createdDate);
    json["ticket-id"] = getValueOfTicketId();
    json["poster-id"] = getValueOfPosterId();
    return json;
}


}  // namespace Model

