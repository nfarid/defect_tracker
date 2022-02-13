
#include "./app.hpp"
#include "./util/misc.hpp"


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
Url parse_url(std::string_view url) {
    // TODO: Improve implementation
    using std::string;
    const size_t len = size(url);

    const auto scheme_endpos = url.find(':');
    if( (scheme_endpos == string::npos) || (scheme_endpos + 3 >= len) ||
                (url[scheme_endpos+1] != '/') || (url[scheme_endpos+2] != '/') )
    {
        throw std::invalid_argument("Invalid Database URL, invalid scheme.");
    }

    const auto username_startpos = scheme_endpos + 3;
    const auto username_endpos = url.find(':', username_startpos);
    if(username_endpos == string::npos)
        throw std::invalid_argument("Invalid Database URL, invalid username.");
    const auto username = url.substr(username_startpos, username_endpos - username_startpos);

    const auto password_startpos = username_endpos + 1;
    const auto password_endpos = url.find('@', password_startpos);
    if(password_endpos == string::npos)
        throw std::invalid_argument("Invalid Database URL, invalid password.");
    const auto password = url.substr(password_startpos, password_endpos - password_startpos);

    const auto host_startpos = password_endpos + 1;
    const auto host_endpos = url.find(':', host_startpos);
    if(host_endpos == string::npos)
        throw std::invalid_argument("Invalid Database URL, invalid host.");
    const auto host = url.substr(host_startpos, host_endpos - host_startpos);

    const auto port_startpos = host_endpos + 1;
    const auto port_endpos = url.find('/', port_startpos);
    if(port_endpos == string::npos)
        throw std::invalid_argument("Invalid Database URL, invalid port.");
    const auto port_str = url.substr(port_startpos, port_endpos - port_startpos);
    const unsigned short port = Util::StrToNum{port_str};

    const auto path_startpos = port_endpos + 1;
    const auto path_endpos = len;
    const auto path = url.substr(path_startpos, path_endpos - path_startpos);

    return {
        string(username),
        string(password),
        string(host),
        port,
        string(path),
    };
}


}  // namespace


drogon::HttpAppFramework& get_app() {
    auto& app = drogon::app();

    try {
        const auto database_url = Util::get_environment("DATABASE");
        const auto parsed_url = parse_url(database_url);

        app.createDbClient(
                "postgresql",
                parsed_url.dbHost,
                parsed_url.dbPort,
                parsed_url.dbName,
                parsed_url.dbUsername,
                parsed_url.dbPassword,
                1,
                "",
                "db"
                );
    }catch(std::exception& ex) {
        std::cerr<<ex.what()<<std::endl;
        std::exit(EXIT_FAILURE);
    }

    app.addListener("127.0.0.1", 3000);
    app.enableSession();

    return app;
}
