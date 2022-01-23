
#include <drogon/HttpAppFramework.h>

#include <iostream>

int main() {
    auto& app = drogon::app();

    std::cout<<"Running app!"<<std::endl;

    drogon::app().addListener("127.0.0.1", 3000);
    app.run();

    return 0;
}
