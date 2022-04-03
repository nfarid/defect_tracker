
#include "auxiliary.hpp"
#include "../util/form_error.hpp"

#include <drogon/MultiPart.h>

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

    const std::optional<int32_t> userId = session.getOptional<int32_t>("user_id");
    if(userId)
        data.insert("session_user_id", std::to_string(*userId) );
    const std::optional<std::string> username = session.getOptional<std::string>("username");
    if(username)
        data.insert("session_username", *username);

    return data;
}

bool isLoggedIn(const Session& session) {
    const std::optional<int32_t> userId = session.getOptional<int32_t>("user_id");
    return userId.has_value();
}

void logIn(drogon::Session& session, int32_t userId, const std::string& username) {
    session.insert("user_id", userId);
    session.insert("username", username);
}

std::unordered_map<std::string, std::string> parseMultiPart(const drogon::HttpRequestPtr& req) {
    std::unordered_map<std::string, std::string> postParams;
    MultiPartParser parser;
    if(parser.parse(req) != 0)
        throw Util::FormError("Cannot parse http request");

    for(const auto& [k, v] : parser.getParameters() )
        postParams[k] = v;

    for(const auto& [key, file] : parser.getFilesMap() ) {
        if(file.getFileType() != FileType::FT_IMAGE) {  // Only image files are accepted
            postParams[key] = "";
            continue;
        }
        const std::string timestamp = trantor::Date::now().toCustomedFormattedString("%s");
        const std::string& filename = timestamp + file.getMd5() + file.getFileName();
        postParams[key] = filename;
        file.saveAs(filename);
    }

    return postParams;
}


}  // namespace Aux

