
#include "../constants.hpp"

#include <drogon/HttpClient.h>
#include <drogon/drogon_test.h>

#include <array>
#include <string>

using namespace drogon;
using std::string_literals::operator""s;

DROGON_TEST(HomeController_IndexGet){
    auto client = HttpClient::newHttpClient(localhost, testPort);

    auto req = HttpRequest::newHttpRequest();

    for(const auto& path : {"/"s, "/index"s, "/home"s}) {
        req->setPath(path);
        client->sendRequest(req, [TEST_CTX](ReqResult res, const HttpResponsePtr& resp){
            // There's nothing we can do if the request didn't reach the server
            // or the server generated garbage.
            REQUIRE(res == ReqResult::Ok);
            REQUIRE(resp != nullptr);

            CHECK(resp->getStatusCode() == k200OK);
            CHECK(resp->contentType() == CT_TEXT_HTML);
        });
    }
}


DROGON_TEST(HomeController_IndexPost){
    auto client = HttpClient::newHttpClient(localhost, testPort);

    auto req = HttpRequest::newHttpFormPostRequest();

    // These paths only allow GET requests
    for(const auto& path : {"/"s, "/index"s, "/home"s}) {
        req->setPath(path);
        client->sendRequest(req, [TEST_CTX](ReqResult res, const HttpResponsePtr& resp){
            REQUIRE(res == ReqResult::Ok);
            REQUIRE(resp != nullptr);

            CHECK(resp->getStatusCode() == k405MethodNotAllowed);
        });
    }
}
