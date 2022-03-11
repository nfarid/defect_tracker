
#include "./auxiliary.hpp"
#include "../constants.hpp"
#include "../models/ticket.hpp"
#include "../models/comment.hpp"

#include <drogon/HttpController.h>


namespace Ctrlr
{


using namespace drogon;
using namespace drogon::orm;
using namespace Aux;


class Ticket : public HttpController<Ticket> {
public:
    /*NO-FORMAT*/
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(Ticket::show, "/ticket/{1}", Get);
    METHOD_LIST_END
    /*YES-FORMAT*/

    void show(const HttpRequestPtr& req, ResponseCallback&& cb, int32_t id);
    Mapper<Model::Ticket> mTicketOrm = Mapper<Model::Ticket>(app().getDbClient("db") );
    Mapper<Model::Comment> mCommentOrm = Mapper<Model::Comment>(app().getDbClient("db") );
};

void Ticket::show(const HttpRequestPtr& req, ResponseCallback&& cb, int32_t id) {
    try {
        // TODO:
        auto ticketFuture = mTicketOrm.findFutureByPrimaryKey(id);
        const Criteria commentCriteria{Model::Comment::Cols::_project, CompareOperator::EQ, id};
        auto commentLstFuture = mCommentOrm.findFutureBy(commentCriteria);

        const auto ticket = ticketFuture.get();
        const auto commentLst = commentLstFuture.get();
        std::vector<Json::Value> commentLstJson{};
        commentLstJson.reserve(size(commentLst) );
        for(const auto& comment : commentLst) {
            auto commentJson = comment.toJson();
            commentJson["created_date"] = comment.getValueOfCreatedDate().toCustomedFormattedString(dateFormat);
            commentLstJson.push_back(std::move(commentJson) );
        }
        auto ticketJson = ticket.toJson();
        ticketJson["created_date"] = ticket.getValueOfCreatedDate().toCustomedFormattedString(dateFormat);

        auto data = getViewData(ticket.getValueOfTitle(), *getSession(req) );
        data.insert("ticket", std::move(ticketJson) );
        data.insert("commentLst", std::move(commentLstJson) );
        return cb(HttpResponse::newHttpViewResponse("ticket.csp", data) );
    } catch(const std::exception& ex) {
        std::cerr<<ex.what()<<std::endl;
        return cb(HttpResponse::newNotFoundResponse() );
    }
}
        return cb(HttpResponse::newNotFoundResponse() );
    }
}


}// namespace Ctrlr

