
#include "../models/ticket.hpp"
#include "../models/_database.hpp"

#include "../util/constants.hpp"

#include <drogon/HttpAppFramework.h>
#include <drogon/HttpClient.h>
#include <drogon/drogon_test.h>
#include <drogon/orm/Exception.h>

#include <string>

using namespace drogon;
using namespace drogon::orm;
using std::string_literals::operator""s;

DROGON_TEST(TicketController_Show){
    auto orm = Mapper<Model::Ticket>(Db::getDb() );
    const Criteria hasTitle{Model::Ticket::Cols::_title, CompareOperator::EQ, "ticket"};
    DrogonDbException ex;
    orm.findOne(hasTitle,
                [TEST_CTX](const Model::Ticket& ticket){
        auto client = HttpClient::newHttpClient(Util::localhost, Util::testPort);
        const Model::Ticket::PrimaryKeyType ticketId = ticket.getValueOfId();

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
