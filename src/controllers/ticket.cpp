
#include "auxiliary.hpp"
#include "../constants.hpp"
#include "../models/account.hpp"
#include "../models/comment.hpp"
#include "../models/project.hpp"
#include "../models/staff.hpp"
#include "../models/ticket.hpp"
#include "../util/misc.hpp"

#include <drogon/HttpController.h>

#include <algorithm>


namespace Ctrlr
{


using namespace drogon;
using namespace drogon::orm;
using namespace Aux;
using std::string_literals::operator""s;


class Ticket : public HttpController<Ticket> {
public:
    /*NO-FORMAT*/
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(Ticket::show, "/ticket/{1}", Get);
        ADD_METHOD_TO(Ticket::newForm, "/project/{1}/report", Get);
        ADD_METHOD_TO(Ticket::edit, "/ticket/{1}/edit", Get);
        ADD_METHOD_TO(Ticket::create, "/project/{1}/report", Post);
        ADD_METHOD_TO(Ticket::update, "/ticket/{1}/edit", Post);
    METHOD_LIST_END
    /*YES-FORMAT*/

    Task<HttpResponsePtr> show(HttpRequestPtr req, int32_t id);
    Task<HttpResponsePtr> newForm(HttpRequestPtr req, int32_t projectId);
    Task<HttpResponsePtr> edit(HttpRequestPtr req, int32_t id);
    Task<HttpResponsePtr> create(HttpRequestPtr req, int32_t projectId);
    Task<HttpResponsePtr> update(HttpRequestPtr req, int32_t id);

private:
    DbClientPtr mDB = app().getDbClient("db");

    CoroMapper<Model::Ticket> mTicketOrm{mDB};
    CoroMapper<Model::Project> mProjectOrm{mDB};
};

Task<HttpResponsePtr> Ticket::show(HttpRequestPtr req, int32_t id) {
    const SessionPtr session = getSession(req);

    try {
        const Model::Ticket ticket = co_await mTicketOrm.findByPrimaryKey(id);
        const Model::Account reporter = co_await ticket.getReporter(mDB);
        const Model::Project project = co_await ticket.getProject(mDB);
        const std::vector commentLst = co_await ticket.getComments(mDB);

        HttpViewData data = getViewData(ticket.getValueOfTitle(), *session);
        data.insert("ticket", ticket.toJson() );
        data.insert("comment_lst", toJson(commentLst) );
        data.insert("project_name", project.getValueOfTitle() );
        data.insert("reporter_username", reporter.getValueOfUsername() );
        if(isLoggedIn(*session) ) {
            const int32_t userId = session->get<int32_t>("user_id");
            data.insert("can_edit", co_await ticket.canEdit(mDB, userId) );
        }

        co_return HttpResponse::newHttpViewResponse("ticket.csp", data);
    } catch(const std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<";"<<__LINE__<<":\n"<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}

Task<HttpResponsePtr> Ticket::newForm(HttpRequestPtr req, int32_t projectId) {
    const SessionPtr session = getSession(req);
    if(!isLoggedIn(*session) ) // Cannot create a new form if not logged in
        co_return HttpResponse::newRedirectionResponse("/");

    try {
        const Model::Project project = co_await mProjectOrm.findByPrimaryKey(projectId);

        HttpViewData data = getViewData("New Ticket", *session);
        data.insert("project", project.toJson() );
        data.insert("severity_lst", Model::Ticket::getSeverityLst() );

        co_return HttpResponse::newHttpViewResponse("ticket_form.csp", data);
    }  catch(const std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<";"<<__LINE__<<":\n"<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}

Task<HttpResponsePtr> Ticket::edit(HttpRequestPtr req, int32_t id) {
    const auto session = getSession(req);
    if(!isLoggedIn(*session) ) // cannot edit if not authenticated
        co_return HttpResponse::newRedirectionResponse("/");
    const int32_t userId = session->get<int32_t>("user_id");

    try {
        const Model::Ticket ticket = co_await mTicketOrm.findByPrimaryKey(id);
        if(!co_await ticket.canEdit(mDB, userId) ) // cannot edit if not authorised
            co_return HttpResponse::newRedirectionResponse("/");

        HttpViewData data = getViewData("Edit Ticket", *session);
        data.insert("ticket_id", std::to_string(id) );
        data.insert("ticket_title", ticket.getValueOfTitle() );
        data.insert("ticket_description", ticket.getValueOfDescription() );
        if(ticket.getAssignedId() )
            data.insert("ticket_assigned", std::to_string(ticket.getValueOfAssignedId() ) );
        data.insert("assignable_lst", co_await ticket.getAssignables(mDB, userId) );
        data.insert("is_reporter", ticket.isReporter(userId) );

        co_return HttpResponse::newHttpViewResponse("ticket_edit.csp", data);
    } catch(const std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<";"<<__LINE__<<":\n"<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}

Task<HttpResponsePtr> Ticket::create(HttpRequestPtr req, int32_t projectId) {
    const SessionPtr session = getSession(req);
    if(!isLoggedIn(*session) ) // Cannot create a ticket if not logged in
        co_return HttpResponse::newRedirectionResponse("/");
    const int32_t userId = session->get<int32_t>("user_id");
    const auto& postParams = req->parameters();

    try {
        co_await Model::Ticket::createTicket(mTicketOrm, postParams, userId, projectId);
        co_return HttpResponse::newRedirectionResponse("/", k303SeeOther);
    }  catch(const std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<";"<<__LINE__<<":\n"<<ex.what()<<std::endl;
        const std::string location = "/project/"s + std::to_string(projectId) + "/report";
        co_return HttpResponse::newRedirectionResponse(location);
    }
}

Task<HttpResponsePtr> Ticket::update(HttpRequestPtr req, int32_t id) {
    const auto session = getSession(req);
    if(!isLoggedIn(*session) ) // cannot edit if not authenticated
        co_return HttpResponse::newRedirectionResponse("/");
    const int32_t userId = session->get<int32_t>("user_id");

    try {
        Model::Ticket ticket = co_await mTicketOrm.findByPrimaryKey(id);
        if(!co_await ticket.canEdit(mDB, userId) ) // cannot edit if not authorised
            co_return HttpResponse::newRedirectionResponse("/");

        const auto& postParams = req->parameters();

        // The ticket's reporter can edit the ticket
        if(ticket.isReporter(userId) )
            ticket.setDescription(postParams.at("form-description") );

        // Can only assign to people in the assingableLst
        const std::vector assingableLst = co_await ticket.getAssignables(mDB, userId);
        const int32_t assignedId = Util::StrToNum{postParams.at("form-assign")};
        for(const auto& staff : toJson(assingableLst) ) {
            if(staff["id"] == assignedId)
                ticket.setAssignedId(assignedId);
        }

        co_await mTicketOrm.update(ticket);

        co_return HttpResponse::newRedirectionResponse("/");
    }  catch(const std::exception& ex) {
        std::cerr<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}


}  // namespace Ctrlr

