
#include "_database.hpp"

#include "../util/core.hpp"
#include "../util/misc.hpp"

#include <drogon/HttpAppFramework.h>
#include <drogon/orm/DbClient.h>

#include <linux/limits.h>    // PATH_MAX
#include <unistd.h>         // readlink (which doesn't null terminate)

#include <array>
#include <filesystem>


namespace Db
{


using namespace drogon;
using namespace drogon::orm;
namespace fs = std::filesystem;
using std::string;
using std::string_view;
using std::string_literals::operator""s;


namespace
{


fs::path get_executable_dir() {
    std::array<char, PATH_MAX> exe_path;
    const ssize_t byte_read = readlink("/proc/self/exe", exe_path.data(), PATH_MAX);
    if( (byte_read <= -1) || (byte_read >= PATH_MAX) )
        throw std::runtime_error("Unable to run readlink(/proc/self/exe)");
    exe_path[static_cast<size_t>(byte_read)] = '\0';  // readlink does not null-terminate, so it must be done manually
    return fs::path{exe_path.data()}.parent_path();
}

// Url format is like this: scheme://username:password@host:port/path
struct DbUrl {
    std::string scheme{};
    std::string username{};
    std::string password{};
    std::string host{};
    std::string port{};
    std::string path{};
    // Does not contain query nor fragment
};

// A simple url parser for parsing a database url
DbUrl parseUrl(string_view urlStr) {
    std::cout<<"Parsing datbase url, should be in format: scheme://username:password@host:port/path"<<std::endl;
    DbUrl parsed;

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
        throw std::invalid_argument("Invalid database url. Invalid DbUrl authority (i.e. userinfo@host:port).");
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
            parsed.port = authority.substr(portStartpos);
        } catch(const std::exception& ex) {
            std::cerr<<ex.what()<<"; Database port is invalid! Assuming port is 5432."<<std::endl;
            parsed.port = "5432";
        }
    } else if(hostStartpos != authority.size() ) {  // There is no ':', so there is no port only host
        parsed.host = authority.substr(hostStartpos);
        std::cerr<<"Database port has not been specified! Assuming port is 5432."<<std::endl;
        parsed.port = "5432";
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


DbClientPtr getDb() {
    // Using a static variable and a iife to make sure that the database connection is opened only once
    const static DbClientPtr db = []() -> DbClientPtr {
        try {
        #ifdef USE_POSTGRESQL
            std::cout<<"Connecting to a postgresql database."<<std::endl;
            Util::CStringView database_url = Util::getEnvironment("DATABASE_URL");
            const DbUrl parsedUrl = parseUrl(database_url);
            const DbClientPtr db_ = DbClient::newPgClient("host="s + parsedUrl.host +
                    " port="s + parsedUrl.port +
                    " dbname="s + parsedUrl.path +
                    " user="s + parsedUrl.username +
                    " password="s + parsedUrl.password,
                    1);
            if(!db_) // should not be null
                throw std::runtime_error("Unable to create a postgresql database client.");
            return db_;
        #else
            std::cout<<"Connecting to sqlite3 database."<<std::endl;
            const fs::path sqlTestDb = get_executable_dir().parent_path() / "sql/test.db";
            const DbClientPtr db_ = DbClient::newSqlite3Client("filename="s + sqlTestDb.string(), 1);
            if(!db_) // should not be null
                throw std::runtime_error("Cannot connect to sqlite3 test.db");
            return db_;
        #endif  // ifdef USE_POSTGRESQL
        } catch(const std::exception& ex) {
            std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<typeid(ex).name()<<" ; "<<ex.what()<<std::endl;
            std::exit(EXIT_FAILURE);
        }
    }();
    return db;
}


}  // namespace Db

