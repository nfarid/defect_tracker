
#include "./app.hpp"
#include "./util/misc.hpp"

#include <sodium.h>


namespace
{


typedef struct Url {
    std::string dbUsername;
    std::string dbPassword;
    std::string dbHost;
    unsigned short dbPort;
    std::string dbName;
} Url;

// A simple url parser for parsing a database url
// Url format is like this: scheme://username:password@host:port/path
Url parseUrl(std::string_view url) {
    // TODO: Improve implementation
    using std::string;
    const size_t len = size(url);

    const auto schemeEndpos = url.find(':');
    if( (schemeEndpos == string::npos) || (schemeEndpos + 3 >= len) ||
                (url[schemeEndpos+1] != '/') || (url[schemeEndpos+2] != '/') )
    {
        throw std::invalid_argument("Invalid Database URL, invalid scheme.");
    }

    const auto usernameStartpos = schemeEndpos + 3;
    const auto usernameEndpos = url.find(':', usernameStartpos);
    if(usernameEndpos == string::npos)
        throw std::invalid_argument("Invalid Database URL, invalid username.");
    const auto username = url.substr(usernameStartpos, usernameEndpos - usernameStartpos);

    const auto passwordStartpos = usernameEndpos + 1;
    const auto passwordEndpos = url.find('@', passwordStartpos);
    if(passwordEndpos == string::npos)
        throw std::invalid_argument("Invalid Database URL, invalid password.");
    const auto password = url.substr(passwordStartpos, passwordEndpos - passwordStartpos);

    const auto hostStartpos = passwordEndpos + 1;
    const auto hostEndpos = url.find(':', hostStartpos);
    if(hostEndpos == string::npos)
        throw std::invalid_argument("Invalid Database URL, invalid host.");
    const auto host = url.substr(hostStartpos, hostEndpos - hostStartpos);

    const auto portStartpos = hostEndpos + 1;
    const auto portEndpos = url.find('/', portStartpos);
    if(portEndpos == string::npos)
        throw std::invalid_argument("Invalid Database URL, invalid port.");
    const auto port_str = url.substr(portStartpos, portEndpos - portStartpos);
    const unsigned short port = Util::StrToNum{port_str};

    const auto pathStartpos = portEndpos + 1;
    const auto pathEndpos = len;
    const auto path = url.substr(pathStartpos, pathEndpos - pathStartpos);

    return {
        string(username),
        string(password),
        string(host),
        port,
        string(path),
    };
}


}  // namespace


drogon::HttpAppFramework& getApp() {
    const auto sodiumError = sodium_init();
    if(sodiumError) {
        std::cerr<<"Sodium failed to initialised!"<<std::endl;
        std::exit(EXIT_FAILURE);
    }

    auto& app = drogon::app();

    try {
        const auto databaseUrl = Util::getEnvironment("DATABASE_URL");
        const auto parsedUrl = parseUrl(databaseUrl);

        app.createDbClient(
                "postgresql",
                parsedUrl.dbHost,
                parsedUrl.dbPort,
                parsedUrl.dbName,
                parsedUrl.dbUsername,
                parsedUrl.dbPassword,
                1,
                "",
                "db"
        );
    }catch(std::exception& ex) {
        std::cerr<<ex.what()<<std::endl;
        std::exit(EXIT_FAILURE);
    }

    const auto portStr = Util::getEnvironment("PORT");
    const unsigned short port = Util::StrToNum{portStr};
    app.addListener("0.0.0.0", port);
    app.enableSession();

    return app;
}
