
// Tell DrogonTest to generate `test::run()`. Only defined this in the main file
#define DROGON_TEST_MAIN

#include "./app.hpp"
#include "./constants.hpp"
#include "./util/misc.hpp"

#include <drogon/drogon_test.h>

#include <cstring>

int main(int argc, char** argv) {
    // Get database url by command line (then try environment)
    std::string_view dbUrl{};
    for(int i=1; i+1<argc; ++i) {
        if(!std::strcmp(argv[i], "--db") )
            dbUrl = argv[i+1];
    }
    if(dbUrl.empty() )
        dbUrl = Util::getEnvironment("DATABASE_URL");

    auto& app = getApp(testPort, dbUrl);
    std::promise<void> promise;
    std::future<void> future = promise.get_future();

    // Start the main loop on another thread
    std::thread thr([& app, & promise](){
        // Queues the promis to be fulfilled after starting the loop
        app.getLoop()->queueInLoop([& promise](){ promise.set_value(); });
        app.run();
    });

    // The future is only satisfied after the event loop started
    future.get();
    const int status = drogon::test::run(argc, argv);

    // Ask the event loop to shutdown and wait
    app.getLoop()->queueInLoop([& app](){ app.quit(); });
    thr.join();
    return status;
}
