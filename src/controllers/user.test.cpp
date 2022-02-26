
 #include "../constants.hpp"
 #include "../models/account.hpp"

 #include <drogon/HttpAppFramework.h>
 #include <drogon/HttpClient.h>
 #include <drogon/drogon_test.h>

 #include <string>

using namespace drogon;
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

DROGON_TEST(UserController_Create){
    auto client = HttpClient::newHttpClient(localhost, testPort);
    client->enableCookies();

    auto createReq = HttpRequest::newHttpFormPostRequest();
    createReq->setPath("/signup");

    const std::string new_username = "new_username";
    createReq->setParameter("signup_username", new_username);
    createReq->setParameter("signup_password", "new_password");

    client->sendRequest(createReq, [TEST_CTX, new_username](ReqResult res, const HttpResponsePtr& resp){
        REQUIRE(res == ReqResult::Ok);
        REQUIRE(resp != nullptr);
        // User is redirected to homepage after signup
        std::cout<<"http: "<<resp->statusCode()<<std::endl;
        for(const auto& [k, v] : resp->getHeaders() )
            std::cout<<k<<" ; "<<v<<std::endl;
        REQUIRE(resp->getHeader("location"s) == "/"s);

        Mapper<Model::Account> orm = Mapper<Model::Account>(app().getDbClient("db") );
        const Criteria userCriteria{Model::Account::Cols::_username, CompareOperator::EQ, new_username};
        REQUIRE(orm.count(userCriteria) == 1);
    });
}
