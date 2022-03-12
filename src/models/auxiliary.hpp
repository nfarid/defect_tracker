
#ifndef HPP_MODELS_AUXILIARY
#define HPP_MODELS_AUXILIARY

#include <json/value.h>

#include <array>
#include <string>



namespace Aux
{


using std::string_literals::operator""s;

const std::array statusLst = {"new"s, "confirmed"s, "unreproducible"s, "resolved"s, "reopened"s};
// Returns a list of statuses{"new","confirmed","unreproducible","resolved","reopened"} as a json
inline Json::Value statusLstJson() {
    const Json::Value static statusLst_ = []{
        Json::Value statusLst_;
        for(const auto& status : statusLst)
            statusLst_.append(status);
        return statusLst_;
    }();
    return statusLst_;
}

const std::array severityLst = {"low"s, "medium"s, "high"s};
// Returns a list of severities{"low","medium","high"} as a json
inline Json::Value severityLstJson() {
    const Json::Value static severityLst_ = []{
        Json::Value severityLst_;
        for(const auto& status : severityLst)
            severityLst_.append(status);
        return severityLst_;
    }();
    return severityLst_;
}


}  // namespace Aux

#endif  // ifndef HPP_MODELS_AUXILIARY
