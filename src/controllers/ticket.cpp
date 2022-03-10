
#include "./auxiliary.hpp"
#include "../constants.hpp"
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
    try {
        const auto ticket = mTicketOrm.findByPrimaryKey(id);
        const std::string& title = ticket.getValueOfTitle();
        const std::string& description = ticket.getValueOfDescr();
        const std::string createdDate = ticket.getValueOfCreatedDate().toCustomedFormattedString(dateFormat);
        const std::string projectId = std::to_string(ticket.getValueOfProject() );
        auto data = getViewData(title, *getSession(req) );
        data.insert("ticket_description", description);
        data.insert("ticket_created_date", createdDate);
        data.insert("ticket_project_id", projectId);
        return cb(HttpResponse::newHttpViewResponse("ticket.csp", data) );
    } catch(const DrogonDbException& ex) {
        std::cerr<<ex.base().what()<<std::endl;
        return cb(HttpResponse::newNotFoundResponse() );
    }
}


}// namespace Ctrlr

