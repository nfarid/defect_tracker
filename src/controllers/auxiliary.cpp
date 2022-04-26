
#include "auxiliary.hpp"
#include "../util/form_error.hpp"

#include <drogon/utils/Utilities.h>

#include <cassert>
#include <string>


namespace Aux
{


using namespace drogon;
using std::string_literals::operator""s;

SessionPtr getSession(const HttpRequestPtr& req) {
    assert(req && "HttpRequestPtr cannot be null");
    SessionPtr session = req->session();
    if(!session) {
        std::cerr<<"Session is not enabled"<<std::endl;
        throw std::runtime_error("Session is not enabled");
    }
    if(!session->find("token") )
        session->insert("token",  drogon::utils::genRandomString(10) );
    return session;
}

HttpViewData getViewData(const std::string& title, const Session& session) {
    HttpViewData data;
    data.insert("title", title);

    data.insert("is-logged-in", isLoggedIn(session) );
    data.insert("session-username", HttpViewData::htmlTranslate(getUsername(session) ) );
    data.insert("session-user-id", getUserId(session) );
    data.insert("token", session.get<std::string>("token") );

    return data;
}

bool isLoggedIn(const Session& session) {
    const std::optional userId = session.getOptional<IdType>("user_id");
    return userId.has_value();
}

IdType getUserId(const Session& session) {
    const std::optional userId = session.getOptional<IdType>("user_id");
    return userId.value_or(0);
}

std::string getUsername(const Session& session) {
    const std::optional username = session.getOptional<std::string>("username");
    return username.value_or("");
}

void logIn(Session& session, IdType userId, const std::string& username) {
    session.insert("user_id", userId);
    session.insert("username", username);
    session.changeSessionIdToClient();
}

void logOut(Session& session) {
    session.clear();
    session.changeSessionIdToClient();
}

std::pair<Util::StringMap, Util::FileMap> parseMultiPart(const HttpRequestPtr& req) {
    Util::StringMap formData;
    MultiPartParser parser;
    if(parser.parse(req) != 0)
        throw Util::FormError("Cannot parse http request");

    for(const auto& [k, v] : parser.getParameters() )
        formData[k] = v;

    for(const auto& [key, file] : parser.getFilesMap() ) {
        const std::string timestamp = trantor::Date::now().toCustomedFormattedString("%s");
        const std::string filename = timestamp + file.getMd5() + "." + std::string(file.getFileExtension() );
        std::cerr<<"filename; "<<filename<<std::endl;
        formData[key] = filename;
    }

    return {formData, parser.getFilesMap()};
}


}  // namespace Aux

