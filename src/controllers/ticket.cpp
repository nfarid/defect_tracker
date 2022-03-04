
#include "./auxiliary.hpp"
#include "../models/ticket.hpp"

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
};

void Ticket::show(const HttpRequestPtr& req, ResponseCallback&& cb, int32_t id) {
    mTicketOrm.findByPrimaryKey(id,
                [=](const Model::Ticket& ticket){
            const std::string& title = ticket.getValueOfTitle();
            const auto data = getViewData(title, *getSession(req) );
            return cb(HttpResponse::newHttpViewResponse("ticket.csp", data) );
        },
                [cb](const DrogonDbException& ex){
            std::cerr<<ex.base().what()<<std::endl;
            return cb(HttpResponse::newNotFoundResponse() );
        }
    );
}


}// namespace Ctrlr

