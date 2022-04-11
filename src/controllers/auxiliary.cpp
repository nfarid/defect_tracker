
#include "auxiliary.hpp"
#include "../util/form_error.hpp"

#include <cassert>
#include <string>


namespace Aux
{


using namespace drogon;

SessionPtr getSession(const HttpRequestPtr& req) {
    assert(req && "HttpRequestPtr cannot be null");
    SessionPtr session = req->session();
    if(!session) {
        std::cerr<<"Session is not enabled"<<std::endl;
        throw std::runtime_error("Session is not enabled");
    }
    return session;
}

HttpViewData getViewData(const std::string& title, const drogon::Session& session) {
    HttpViewData data;
    data.insert("title", title);

    data.insert("is-logged-in", isLoggedIn(session) );
    data.insert("session-username", HttpViewData::htmlTranslate(getUsername(session) ) );
    data.insert("session-user-id", getUserId(session) );

    return data;
}

bool isLoggedIn(const Session& session) {
    const std::optional<int32_t> userId = session.getOptional<int32_t>("user_id");
    return userId.has_value();
}

int32_t getUserId(const Session& session) {
    const std::optional<int32_t> userId = session.getOptional<int32_t>("user_id");
    return userId.value_or(0);
}

std::string getUsername(const drogon::Session& session) {
    const std::optional<std::string> username = session.getOptional<std::string>("username");
    return username.value_or("");
}

void logIn(drogon::Session& session, int32_t userId, const std::string& username) {
    session.insert("user_id", userId);
    session.insert("username", username);
}

std::pair<Util::StringMap, Util::FileMap> parseMultiPart(const drogon::HttpRequestPtr& req) {
    Util::StringMap postParams;
    MultiPartParser parser;
    if(parser.parse(req) != 0)
        throw Util::FormError("Cannot parse http request");

    for(const auto& [k, v] : parser.getParameters() )
        postParams[k] = v;

    for(const auto& [key, file] : parser.getFilesMap() ) {
        const std::string timestamp = trantor::Date::now().toCustomedFormattedString("%s");
        const std::string& filename = timestamp + file.getMd5() + file.getFileName();
        postParams[key] = filename;
    }

    return {postParams, parser.getFilesMap()};
}


}  // namespace Aux

