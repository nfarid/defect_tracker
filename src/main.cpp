
#include "util/database.hpp"

#include "util/misc.hpp"

#include <drogon/HttpAppFramework.h>

using drogon::app;

int main(int argc, char** argv) {
    unsigned short port = 3000;
    try {
        const Util::CStringView portStr = Util::getEnvironment("PORT");
        port = Util::StrToNum{portStr};
    }  catch(const std::exception& ex) {
        std::cerr<<"Port not specified! Defaulting to 3000"<<std::endl;
    }
    std::cout<<"Listening to port "<<port<<std::endl;

    app().addListener("0.0.0.0", port);
    app().enableSession();

    app().run();

    return EXIT_SUCCESS;
}
