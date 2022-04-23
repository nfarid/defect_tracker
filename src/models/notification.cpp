
#include "notification.hpp"

#include <drogon/HttpViewData.h>


namespace Model
{


using namespace drogon;
using Json::UInt64;


Json::Value Model::Notification::toViewJson() const {
    Json::Value json;
    json["id"] = static_cast<UInt64>(getValueOfId() );
    json["summary"] = HttpViewData::htmlTranslate(getValueOfSummary() );
    json["user-id"] = static_cast<UInt64>(getValueOfUserId() );
    json["ticket-id"] = static_cast<UInt64>(getValueOfTicketId() );
    return json;
}


}  // namespace Model

