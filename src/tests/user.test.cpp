
#include "../models/account.hpp"

#include "../util/constants.hpp"
#include "../util/coroutine.hpp"
#include "../util/database.hpp"

#include <drogon/HttpAppFramework.h>
#include <drogon/HttpClient.h>
#include <drogon/drogon_test.h>

#include <string>

using namespace drogon;
using namespace drogon::orm;
using std::string_literals::operator""s;

DROGON_TEST(UserController_New){
    drogon::sync_wait([TEST_CTX]()-> Task<> {
        // Check if the signup form is correct
        auto client = HttpClient::newHttpClient(Util::localhost, Util::testPort);
        client->enableCookies();

        auto newReq = HttpRequest::newHttpRequest();
        newReq->setPath("/signup");

        try {
            const HttpResponsePtr resp = co_await client->sendRequestCoro(newReq);

            CO_REQUIRE(resp != nullptr);
            CHECK(resp->getStatusCode() == k200OK);
            CHECK(resp->contentType() == CT_TEXT_HTML);
        }  catch(const std::exception& ex) {
            FAIL(ex.what() );
        }
    }() );
}

DROGON_TEST(UserController_Create){
    drogon::sync_wait([TEST_CTX]()-> Task<> {
        // Emulate a signup form
        auto client = HttpClient::newHttpClient(Util::localhost, Util::testPort);
        client->enableCookies();

        auto createReq = HttpRequest::newHttpFormPostRequest();
        createReq->setPath("/signup");

        const std::string new_username = "new_username";
        createReq->setParameter("form-username", new_username);
        createReq->setParameter("form-password", "new_password");

        const Criteria hasUsername{Model::Account::Cols::_username, CompareOperator::EQ, new_username};

        try {
            const HttpResponsePtr resp = co_await client->sendRequestCoro(createReq);

            CO_REQUIRE(resp != nullptr);
            // User is redirected to homepage after signup
            CHECK(resp->getHeader("location"s) == "/"s);
        }  catch(const std::exception& ex) {
            FAIL(ex.what() );
        }

        // User should be in the database
        CoroMapper<Model::Account> accountOrm = Util::getDb();
        const auto foundUsersAfterCreation = co_await accountOrm.findBy(hasUsername);
        CO_REQUIRE(size(foundUsersAfterCreation) == 1);                    // there should be 1 new user with the username

        // Now manually deleting the new user
        const auto userId = foundUsersAfterCreation.front().getValueOfId();
        co_await accountOrm.deleteByPrimaryKey(userId);
        const auto foundUsersAfterDeletion = co_await accountOrm.findBy(hasUsername);
        CHECK( foundUsersAfterDeletion.empty() );  // User should not be in the database
    }() );
}
