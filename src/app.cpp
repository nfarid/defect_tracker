
#include "app.hpp"

#include "util/misc.hpp"

#include <sodium.h>


using std::string;
using std::string_view;


namespace
{


// Url format is like this: scheme://username:password@host:port/path
typedef struct Url {
    string scheme{};
    string username{};
    string password{};
    string host{};
    unsigned short port{};
    string path{};
    // Does not contain query nor fragment
} Url;

// A simple url parser for parsing a database url
Url parseUrl(string_view urlString);


}  // namespace


drogon::HttpAppFramework& getApp(unsigned short port, string_view dbUrl) {
    const auto sodiumError = sodium_init();
    if(sodiumError) {
        std::cerr<<"Sodium failed to initialised! "<<sodiumError<<std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::cout<<"Starting the web application"<<std::endl;
    auto& app = drogon::app();

    try {
        const auto parsedUrl = parseUrl(dbUrl);
        app.createDbClient(
                "postgresql",
                parsedUrl.host,
                parsedUrl.port,
                parsedUrl.path,
                parsedUrl.username,
                parsedUrl.password,
                1,
                "",
                "db"
        );
    }catch(std::exception& ex) {
        std::cerr<<"Unable to connect to database: "<<std::endl;
        std::cerr<<ex.what()<<std::endl;
        std::cerr<<"Database url must be in format: \n"<<"scheme://username:password@host:port/path"<<std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::cout<<"Listening at port "<<port<<std::endl;
    app.addListener("0.0.0.0", port);
    app.enableSession();

    return app;
}


namespace
{


Url parseUrl(string_view urlStr) {
    std::cout<<"Parsing datbase url, should be in format: scheme://username:password@host:port/path"<<std::endl;
    Url parsed;

    // Obtaining the scheme
    size_t schemeEndpos = urlStr.find("://");
    size_t authorityStartpos = schemeEndpos+3;
    if(schemeEndpos != string::npos) {
        parsed.scheme = urlStr.substr(0, schemeEndpos);
    } else {
        std::cerr<<"Database scheme has not been specified! Assuming scheme is 'postgres'."<<std::endl;
        parsed.scheme = "postgres";
        schemeEndpos = 0;
        authorityStartpos = 0;
    }

    // Obtaining the authority (i.e. userinfo@host:port)
    const size_t authorityEndpos = urlStr.find('/', authorityStartpos);
    if(authorityEndpos == string::npos)
        throw std::invalid_argument("Invalid database url. Invalid url authority (i.e. userinfo@host:port).");
    const string_view authority = urlStr.substr(authorityStartpos, authorityEndpos - authorityStartpos);

    // Obtaining the userinfo (i.e. username:password)
    size_t userinfoEndpos = authority.find('@');
    size_t hostStartpos = userinfoEndpos + 1;
    if(userinfoEndpos != string::npos) {
        // Obtaining the username and password from userinfo
        const string_view userinfo = authority.substr(0, userinfoEndpos);
        const size_t usernameEndpos = userinfo.find(':');
        parsed.username = userinfo.substr(0, usernameEndpos);
        if(usernameEndpos != string::npos) {
            const size_t passwordStartpos = usernameEndpos + 1;
            parsed.password = userinfo.substr(passwordStartpos);
        }
    } else {
        userinfoEndpos = 0;
        hostStartpos = 0;
        std::cerr<<"Database userinfo has not been specified! Assuming empty userinfo."<<std::endl;
    }

    // Obtain host:port
    const size_t hostEndpos = authority.find(':', hostStartpos);
    if(hostEndpos != string::npos) {
        parsed.host = authority.substr(hostStartpos, hostEndpos - hostStartpos);
        const size_t portStartpos = hostEndpos + 1;
        try {
            parsed.port = Util::StrToNum{authority.substr(portStartpos)};
        } catch(const std::exception& ex) {
            std::cerr<<ex.what()<<"; Database port is invalid! Assuming port is 5432."<<std::endl;
            parsed.port = 5432;
        }
    } else if(hostStartpos != authority.size() ) {  // There is no ':', so there is no port only host
        parsed.host = authority.substr(hostStartpos);
        std::cerr<<"Database port has not been specified! Assuming port is 5432."<<std::endl;
        parsed.port = 5432;
    } else {
        std::cerr<<"Database host has not been specified! Assuming host is 'localhost'."<<std::endl;
        parsed.host = "localhost";
    }

    // Obtaining the path
    const size_t pathStartpos = authorityEndpos + 1;
    parsed.path = urlStr.substr(pathStartpos);

    return parsed;
}


}  // namespace

