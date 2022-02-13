
#include <drogon/HttpClient.h>
#include <drogon/drogon_test.h>

using namespace drogon;

DROGON_TEST(HomeControllerTest){
    auto client = HttpClient::newHttpClient("http://localhost:3000");
    auto req = HttpRequest::newHttpRequest();
    req->setPath("/");
    client->sendRequest(req, [TEST_CTX](ReqResult res, const HttpResponsePtr& resp){
        // There's nothing we can do if the request didn't reach the server
        // or the server generated garbage.
        REQUIRE(res == ReqResult::Ok);
        REQUIRE(resp != nullptr);

        CHECK(resp->getStatusCode() == k200OK);
        CHECK(resp->contentType() == CT_TEXT_HTML);
    });
}
