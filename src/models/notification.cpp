
#include "notification.hpp"

#include "../util/database.hpp"

#include <drogon/HttpViewData.h>


namespace Model
{


using namespace drogon;
using namespace drogon::orm;
using Json::UInt64;


drogon::Task<Notification> Notification::findByPrimaryKey(PrimaryKeyType notificationId) {
    CoroMapper<Notification> notificationOrm = Util::getDb();
    co_return co_await notificationOrm.findByPrimaryKey(notificationId);
}

drogon::Task<> Notification::deleteByPrimaryKey(PrimaryKeyType notificationId) {
    CoroMapper<Notification> notificationOrm = Util::getDb();
    co_await notificationOrm.deleteByPrimaryKey(notificationId);
}

Json::Value Model::Notification::toViewJson() const {
    Json::Value json;
    json["id"] = static_cast<UInt64>(getValueOfId() );
    json["summary"] = HttpViewData::htmlTranslate(getValueOfSummary() );
    json["user-id"] = static_cast<UInt64>(getValueOfUserId() );
    json["ticket-id"] = static_cast<UInt64>(getValueOfTicketId() );
    return json;
}


}  // namespace Model

