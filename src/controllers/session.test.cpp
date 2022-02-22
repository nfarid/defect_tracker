
#include "../constants.hpp"

#include <drogon/HttpClient.h>
#include <drogon/drogon_test.h>

using namespace drogon;

DROGON_TEST(SessionController_LoginGet){
    auto client = HttpClient::newHttpClient(localhost, testPort);

    auto req = HttpRequest::newHttpRequest();
    req->setPath("/login");

    client->sendRequest(req, [TEST_CTX](ReqResult res, const HttpResponsePtr& resp){
        REQUIRE(res == ReqResult::Ok);
        REQUIRE(resp != nullptr);

        CHECK(resp->getStatusCode() == k200OK);
        CHECK(resp->contentType() == CT_TEXT_HTML);
    });
}
