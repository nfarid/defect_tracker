
#include "../constants.hpp"

#include <drogon/HttpClient.h>
#include <drogon/drogon_test.h>

#include <string>

using namespace drogon;
using std::string_literals::operator""s;

// Test when the user goes to the login page
DROGON_TEST(UserSessionController_New){
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

// Test when a correct user session is created
DROGON_TEST(UserSessionController_Create){
    auto client = HttpClient::newHttpClient(localhost, testPort);
    client->enableCookies();

    auto createReq = HttpRequest::newHttpFormPostRequest();
    createReq->setPath("/login");

    // This presume that there's a user with username of username and password digest of password in the test database
    createReq->setParameter("login_username", "username");
    createReq->setParameter("login_password", "password");

    client->sendRequest(createReq, [TEST_CTX, client](ReqResult res, const HttpResponsePtr& resp){
        REQUIRE(res == ReqResult::Ok);
        REQUIRE(resp != nullptr);

        // Once a user session is created, redirect back to the homepage
        CHECK(resp->getStatusCode() == k201Created);
        CHECK(resp->getHeader("location"s) == "/"s);

        // Test when the user tries to login twice in a row
        // Try logging in again
        auto loginAgainReq = HttpRequest::newHttpRequest();
        loginAgainReq->setPath("/login");

        client->sendRequest(loginAgainReq, [TEST_CTX](ReqResult res, const HttpResponsePtr& resp){
            REQUIRE(res == ReqResult::Ok);
            REQUIRE(resp != nullptr);

            // If the user tries to log in again, then redirect them to the homepage rather than the login page
            CHECK(resp->getStatusCode() == k302Found);
            CHECK(resp->getHeader("location"s) == "/"s);
        });
    });
}

// Test when a user tries to create a session with the wrong details
DROGON_TEST(UserSessionController_CreateIncorrect){
    auto client = HttpClient::newHttpClient(localhost, testPort);
    client->enableCookies();

    auto createReq = HttpRequest::newHttpFormPostRequest();
    createReq->setPath("/login");

    // This presume that there isn't a user with username of wrong username and password digest of wrong password in the test database
    createReq->setParameter("login_username", "wrong username");
    createReq->setParameter("login_password", "wrong password");

    client->sendRequest(createReq, [TEST_CTX, client](ReqResult res, const HttpResponsePtr& resp){
        REQUIRE(res == ReqResult::Ok);
        REQUIRE(resp != nullptr);

        // Should not create a new user session if account details are wrong
        CHECK(resp->getStatusCode() != k201Created);
    });
}
