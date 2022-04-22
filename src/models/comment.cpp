
#include "comment.hpp"

#include "../util/constants.hpp"
#include "../util/database.hpp"
#include "../util/string.hpp"

#include <drogon/HttpAppFramework.h>
#include <drogon/HttpViewData.h>


namespace Model
{


using namespace drogon;
using namespace drogon::orm;


drogon::Task<Comment> Comment::createComment(const Util::StringMap& postParams, PrimaryKeyType userId,
        PrimaryKeyType ticketId)
{
    CoroMapper<Comment> orm = Util::getDb();

    Model::Comment newComment{};
    newComment.setPost(Util::getTrimmed(postParams.at("form-post") ) );
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

