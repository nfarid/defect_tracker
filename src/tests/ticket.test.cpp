
#include "../util/constants.hpp"
#include "../models/ticket.hpp"

#include <drogon/HttpAppFramework.h>
#include <drogon/HttpClient.h>
#include <drogon/drogon_test.h>
#include <drogon/orm/Exception.h>

#include <string>

using namespace drogon;
using namespace drogon::orm;
using std::string_literals::operator""s;

DROGON_TEST(TicketController_Show){
    auto orm = Mapper<Model::Ticket>(app().getDbClient("db") );
    const Criteria hasTitle{Model::Ticket::Cols::_title, CompareOperator::EQ, "ticket"};
    DrogonDbException ex;
    orm.findOne(hasTitle,
                [TEST_CTX](const Model::Ticket& ticket){
        auto client = HttpClient::newHttpClient(localhost, testPort);
        const int32_t ticketId = ticket.getValueOfId();

        auto showReq = HttpRequest::newHttpRequest();
        showReq->setPath("/ticket/"s + std::to_string(ticketId) );

        client->sendRequest(showReq, [TEST_CTX](ReqResult res, const HttpResponsePtr& resp){
            REQUIRE(res == ReqResult::Ok);
            REQUIRE(resp != nullptr);
            REQUIRE(resp->getStatusCode() == k200OK);
            REQUIRE(resp->contentType() == CT_TEXT_HTML);
        });
    },
                [TEST_CTX](const DrogonDbException& ex){
        std::cerr<<ex.base().what()<<std::endl;
        REQUIRE(false && ex.base().what() );
    });
}
