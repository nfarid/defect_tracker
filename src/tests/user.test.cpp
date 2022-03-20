
 #include "../constants.hpp"
 #include "../models/account.hpp"

 #include <drogon/HttpAppFramework.h>
 #include <drogon/HttpClient.h>
 #include <drogon/drogon_test.h>

 #include <string>

using namespace drogon;
using namespace drogon::orm;
using std::string_literals::operator""s;

DROGON_TEST(UserController_New){
    auto client = HttpClient::newHttpClient(localhost, testPort);
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
    auto client = HttpClient::newHttpClient(localhost, testPort);
    client->enableCookies();

    auto createReq = HttpRequest::newHttpFormPostRequest();
    createReq->setPath("/signup");

    const std::string new_username = "new_username";
    createReq->setParameter("form-username", new_username);
    createReq->setParameter("form-password", "new_password");

    client->sendRequest(createReq, [TEST_CTX, new_username, client](ReqResult res, const HttpResponsePtr& resp){
        REQUIRE(res == ReqResult::Ok);
        REQUIRE(resp != nullptr);
        // User is redirected to homepage after signup
        REQUIRE(resp->getHeader("location"s) == "/"s);

        // The user should be in the database
        Mapper<Model::Account> orm = Mapper<Model::Account>(app().getDbClient("db") );
        const Criteria userCriteria{Model::Account::Cols::_username, CompareOperator::EQ, new_username};
        const auto foundUserLst = orm.findBy(userCriteria);
        REQUIRE(size(foundUserLst) == 1);

        const auto id = foundUserLst.front().getValueOfId();
        // Delete the user
        auto deleteReq = HttpRequest::newHttpFormPostRequest();
        deleteReq->setPath("/user/"s + std::to_string(id) + "/delete");
        client->sendRequest(deleteReq, [TEST_CTX, userCriteria, orm]
        (ReqResult res, const HttpResponsePtr& resp) mutable{
            REQUIRE(res == ReqResult::Ok);
            REQUIRE(resp != nullptr);
            REQUIRE(resp->getHeader("location"s) == "/"s);
            // User should not be in the database
            REQUIRE(orm.count(userCriteria) == 0);
        });
    });
}
