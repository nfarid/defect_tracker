
#include "notification.hpp"

#include <drogon/HttpViewData.h>


namespace Model
{


using namespace drogon;


Json::Value Model::Notification::toViewJson() const {
    Json::Value json;
    json["id"] = getValueOfId();
    json["summary"] = HttpViewData::htmlTranslate(getValueOfSummary() );
    json["user-id"] = getValueOfUserId();
    json["ticket-id"] = getValueOfTicketId();
    return json;
}


}  // namespace Model

