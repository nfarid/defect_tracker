
#include "../constants.hpp"
#include "../models/project.hpp"

#include <drogon/HttpAppFramework.h>
#include <drogon/HttpClient.h>
#include <drogon/drogon_test.h>

#include <string>

using namespace drogon;
using namespace drogon::orm;
using std::string_literals::operator""s;

DROGON_TEST(ProjectController_Search){
    auto client = HttpClient::newHttpClient(localhost, testPort);

    auto searchReq = HttpRequest::newHttpRequest();
    searchReq->setPath("/search?search-project=project");

    client->sendRequest(searchReq, [TEST_CTX](ReqResult res, const HttpResponsePtr& resp){
        REQUIRE(res == ReqResult::Ok);
        REQUIRE(resp != nullptr);
        // User can search for project
        REQUIRE(resp->getStatusCode() == k200OK);
        REQUIRE(resp->contentType() == CT_TEXT_HTML);
    });
}
