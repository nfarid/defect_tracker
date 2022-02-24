
#include "./app.hpp"
#include "./util/misc.hpp"

#include <cstring>
#include <string_view>

int main(int argc, char** argv) {
    std::string_view portStr{};
    std::string_view dbUrl{};

    // Parse command line arguments
    // E.g. if arguments are ./app  --port 5000;   then set portStr to 5000
    for(int i=1; i+1<argc; ++i) {
        if(!strcmp(argv[i], "--port") )
            portStr = argv[i+1];
        if(!strcmp(argv[i], "--db") )
            dbUrl = argv[i+1];
    }

    // If not passed by command line then use environment
    if(portStr.empty() ) {
        try {
            portStr = Util::getEnvironment("PORT");
        } catch(std::exception& ex) {
            std::cerr<<ex.what()<<std::endl;
            std::cerr<<"No port specified, defaulting to 3000"<<std::endl;
            portStr = "3000";
        }
    }
    unsigned short port = 3000;
    try {
        port = Util::StrToNum{portStr};
    } catch(std::exception& ex) {
        std::cerr<<ex.what()<<std::endl;
        std::cerr<<"Invalid port specified, defaulting to 3000"<<std::endl;
    }

    try {
        if(dbUrl.empty() )
            dbUrl = Util::getEnvironment("DATABASE_URL");
    } catch(std::exception& ex) {
        std::cerr<<ex.what()<<std::endl;
        std::cerr<<"Database url not specified"<<std::endl;
        std::cerr<<"Either set DATABASE_URL environment or pass in --db parameter"<<std::endl;
        return EXIT_FAILURE;
    }

    auto& app = getApp(port, dbUrl);
    app.run();

    return EXIT_SUCCESS;
}
