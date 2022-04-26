
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
using Json::UInt64;


drogon::Task<Comment> Comment::createComment(const Util::StringMap& formData, PrimaryKeyType userId,
        PrimaryKeyType ticketId)
{
    CoroMapper<Comment> orm = Util::getDb();

    Model::Comment newComment{};
    newComment.setPost(Util::getTrimmed(formData.at("form-post") ) );
    newComment.setCreatedDate(trantor::Date::now() );
    newComment.setPosterId(userId);
    newComment.setTicketId(ticketId);
    co_return co_await orm.insert(newComment);
}

Json::Value Comment::toViewJson() const {
    Json::Value json{};
    json["id"] = static_cast<UInt64>(getValueOfId() );
    json["post"] = HttpViewData::htmlTranslate(getValueOfPost() );
    const auto createdDate = getValueOfCreatedDate().toCustomedFormattedString(Util::dateFormat);
    json["created-date"] = HttpViewData::htmlTranslate(createdDate);
    json["ticket-id"] = static_cast<UInt64>(getValueOfTicketId() );
    json["poster-id"] = static_cast<UInt64>(getValueOfPosterId() );
    return json;
}


}  // namespace Model

