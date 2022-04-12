
#include "../util/constants.hpp"
#include "../models/account.hpp"

#include <drogon/HttpAppFramework.h>
#include <drogon/HttpClient.h>
#include <drogon/drogon_test.h>

#include <string>

using namespace drogon;
using namespace drogon::orm;
using std::string_literals::operator""s;

DROGON_TEST(UserController_New){
    // Check if the signup form is correct
    auto client = HttpClient::newHttpClient(Util::localhost, Util::testPort);
    client->enableCookies();

    auto newReq = HttpRequest::newHttpRequest();
    newReq->setPath("/signup");

    client->sendRequest(newReq, [TEST_CTX](ReqResult res, const HttpResponsePtr& resp){
        REQUIRE(res == ReqResult::Ok);
        REQUIRE(resp != nullptr);
        // User can access the signup page if they haven't logged in
        REQUIRE(resp->getStatusCode() == k200OK);
        REQUIRE(resp->contentType() == CT_TEXT_HTML);
    });
}

DROGON_TEST(UserController_CreateAndDestroy){
    // Emulate a signup form
    auto client = HttpClient::newHttpClient(Util::localhost, Util::testPort);
    client->enableCookies();

    auto createReq = HttpRequest::newHttpFormPostRequest();
    createReq->setPath("/signup");

    const std::string new_username = "new_username";
    createReq->setParameter("form-username", new_username);
    createReq->setParameter("form-password", "new_password");

    // Send a signup request
    client->sendRequest(createReq, [TEST_CTX, new_username, client](ReqResult res, const HttpResponsePtr& resp){
        REQUIRE(res == ReqResult::Ok);
        REQUIRE(resp != nullptr);
        // User is redirected to homepage after signup
        REQUIRE(resp->getHeader("location"s) == "/"s);

        sync_wait([TEST_CTX, new_username]() -> Task<> {
            // User should be in the database
            CoroMapper<Model::Account> accountOrm = app().getDbClient("db");
            const Criteria hasUsername{Model::Account::Cols::_username, CompareOperator::EQ, new_username};
            const auto foundUsers = co_await accountOrm.findBy(hasUsername);
            CO_REQUIRE(size(foundUsers) == 1);  // there should be 1 new user with the username

            // Deleting the new user
            const auto userId = foundUsers.front().getValueOfId();
            co_await accountOrm.deleteByPrimaryKey(userId);
            CO_REQUIRE( (co_await accountOrm.findBy(hasUsername) ).empty() );  // User should not be in the database
        }() );
    });
}
