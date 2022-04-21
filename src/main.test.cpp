
// Tell DrogonTest to generate `test::run()`. Only defined this in the main file
#define DROGON_TEST_MAIN

#include "util/constants.hpp"
#include "util/misc.hpp"

#include <drogon/drogon_test.h>
#include <drogon/HttpAppFramework.h>

#include <cstring>

int main(int argc, char** argv) {
    std::cout<<"Starting the tests, make sure that create data sql script has been ran before"<<std::endl;

    auto& app = drogon::app();
    app.addListener("0.0.0.0", 3000);
    app.enableSession();

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
