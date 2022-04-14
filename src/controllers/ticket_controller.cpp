
#include "auxiliary.hpp"
#include "../models/account.hpp"
#include "../models/comment.hpp"
#include "../models/notification.hpp"
#include "../models/project.hpp"
#include "../models/staff.hpp"
#include "../models/ticket.hpp"
#include "../util/form_error.hpp"
#include "../util/misc.hpp"

#include <drogon/HttpController.h>

#include <algorithm>


namespace Ctrlr
{


using namespace drogon;
using namespace drogon::orm;
using namespace Aux;
using std::string_literals::operator""s;


class TicketController : public HttpController<TicketController> {
public:
    /*NO-FORMAT*/
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(TicketController::show, "/ticket/{1}", Get);
        ADD_METHOD_TO(TicketController::newForm, "/project/{1}/report", Get);
        ADD_METHOD_TO(TicketController::edit, "/ticket/{1}/edit", Get);
        ADD_METHOD_TO(TicketController::create, "/project/{1}/report", Post);
        ADD_METHOD_TO(TicketController::update, "/ticket/{1}/edit", Post);
        ADD_METHOD_TO(TicketController::destroy, "/ticket/{1}/delete", Post);
        ADD_METHOD_TO(TicketController::throughNotification, "/notification/{1}", Post);
    METHOD_LIST_END
    /*YES-FORMAT*/

    Task<HttpResponsePtr> show(HttpRequestPtr req, int32_t id);
    Task<HttpResponsePtr> newForm(HttpRequestPtr req, int32_t projectId);
    Task<HttpResponsePtr> edit(HttpRequestPtr req, int32_t id);
    Task<HttpResponsePtr> create(HttpRequestPtr req, int32_t projectId);
    Task<HttpResponsePtr> update(HttpRequestPtr req, int32_t id);
    Task<HttpResponsePtr> destroy(HttpRequestPtr req, int32_t id);
    Task<HttpResponsePtr> throughNotification(HttpRequestPtr req, int32_t notificationId);

private:
    DbClientPtr mDB = app().getDbClient("db");

    CoroMapper<Model::Ticket> mTicketOrm{mDB};
    CoroMapper<Model::Project> mProjectOrm{mDB};
    CoroMapper<Model::Notification> mNotifcationOrm{mDB};

    Task<HttpResponsePtr> newImpl(HttpRequestPtr req, int32_t projectId,
            Util::StringMap formData, std::string errorMessage);
};

Task<HttpResponsePtr> TicketController::show(HttpRequestPtr req, int32_t id) {
    const SessionPtr session = getSession(req);

    try {
        const Model::Ticket ticket = co_await mTicketOrm.findByPrimaryKey(id);
        const auto reporterAwaiter = ticket.getReporter();
        const auto projectAwaiter = ticket.getProject();
        const auto commentLstAwaiter = ticket.getComments();
        const Model::Account reporter = co_await reporterAwaiter;
        const Model::Project project = co_await projectAwaiter;
        const std::vector commentLst = co_await commentLstAwaiter;

        HttpViewData data = getViewData(ticket.getValueOfTitle(), *session);
        data.insert("ticket", ticket.toViewJson() );
        data.insert("project", project.toViewJson() );
        data.insert("reporter", reporter.toViewJson() );
        data.insert("comment-lst", toViewJson(commentLst) );
        if(isLoggedIn(*session) ) {
            const int32_t userId = getUserId(*session);
            data.insert("can-edit", co_await ticket.canEdit(userId) );
            data.insert("can-delete", ticket.isReporter(userId) );
        }

        co_return HttpResponse::newHttpViewResponse("ticket.csp", data);
    } catch(const std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<";"<<__LINE__<<":\n"<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}

Task<HttpResponsePtr> TicketController::newForm(HttpRequestPtr req, int32_t projectId) {
    return newImpl(req, projectId, {}, "");
}

Task<HttpResponsePtr> TicketController::newImpl(HttpRequestPtr req, int32_t projectId,
        Util::StringMap formData, std::string errorMessage)
{
    const SessionPtr session = getSession(req);
    if(!isLoggedIn(*session) ) // Cannot create a new form if not logged in
        co_return HttpResponse::newRedirectionResponse("/");

    try {
        const Model::Project project = co_await mProjectOrm.findByPrimaryKey(projectId);

        HttpViewData data = getViewData("New Ticket", *session);
        data.insert("project", project.toViewJson() );
        data.insert("severity-lst", Model::Ticket::getSeverityLst() );
        data.insert("form-error", errorMessage);
        for(const auto& [k, v] : formData)
            data.insert(k, HttpViewData::htmlTranslate(v) );

        co_return HttpResponse::newHttpViewResponse("ticket_form.csp", data);
    }  catch(const std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<";"<<__LINE__<<":\n"<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}

Task<HttpResponsePtr> TicketController::edit(HttpRequestPtr req, int32_t id) {
    const auto session = getSession(req);
    if(!isLoggedIn(*session) ) // cannot edit if not authenticated
        co_return HttpResponse::newRedirectionResponse("/");
    const int32_t userId = getUserId(*session);

    try {
        const Model::Ticket ticket = co_await mTicketOrm.findByPrimaryKey(id);
        const Model::Project project = co_await ticket.getProject();
        if(!co_await ticket.canEdit(userId) ) // cannot edit if not authorised
            co_return HttpResponse::newRedirectionResponse("/");

        HttpViewData data = getViewData("Edit Ticket", *session);
        data.insert("ticket_id", std::to_string(id) );
        data.insert("ticket_title", ticket.getValueOfTitle() );
        data.insert("ticket_description", ticket.getValueOfDescription() );
        if(ticket.getAssignedId() )
            data.insert("ticket_assigned_id", ticket.getValueOfAssignedId() );
        data.insert("assignable_lst",  toViewJson(co_await ticket.getAssignables(userId) ) );
        data.insert("is_reporter", ticket.isReporter(userId) );
        data.insert("is_staff", co_await project.isStaff(userId) );
        data.insert("status_lst", Model::Ticket::getStatusLst() );
        data.insert("severity-lst", Model::Ticket::getSeverityLst() );

        co_return HttpResponse::newHttpViewResponse("ticket_edit.csp", data);
    } catch(const std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<";"<<__LINE__<<":\n"<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}

Task<HttpResponsePtr> TicketController::create(HttpRequestPtr req, int32_t projectId) {
    const SessionPtr session = getSession(req);
    if(!isLoggedIn(*session) ) // Cannot create a ticket if not logged in
        co_return HttpResponse::newRedirectionResponse("/");
    const int32_t userId = getUserId(*session);
    const auto& [postParams, fileParams] = parseMultiPart(req);

    std::optional<Task<HttpResponsePtr> > retryForm;

    try {
        co_await Model::Ticket::createTicket(postParams, fileParams, userId, projectId);
        co_return HttpResponse::newRedirectionResponse("/", k303SeeOther);
    }  catch(const Util::FormError& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<";"<<__LINE__<<":\n"<<ex.what()<<std::endl;
        // If there's a form error, then retry the ticket creation form
        retryForm = newImpl(req, projectId, postParams, ex.what() );
        // co_await newImpl(req, projectId, postParams, ex.what() ) cannot be done in catch handler
    } catch(const std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<";"<<__LINE__<<":\n"<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
    co_return co_await retryForm.value();
}

Task<HttpResponsePtr> TicketController::update(HttpRequestPtr req, int32_t id) {
    const auto session = getSession(req);
    if(!isLoggedIn(*session) ) // cannot edit if not authenticated
        co_return HttpResponse::newRedirectionResponse("/");
    const int32_t userId = getUserId(*session);

    const auto& postParams = req->parameters();
    std::optional<Task<HttpResponsePtr> > retryForm;
    try {
        Model::Ticket ticket = co_await mTicketOrm.findByPrimaryKey(id);
        co_await ticket.update(postParams, userId);
        co_return HttpResponse::newRedirectionResponse("/");
    } catch(const Util::FormError& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<";"<<__LINE__<<":\n"<<ex.what()<<std::endl;
        retryForm = edit(req, id);  // If there's a form error, then retry
    }  catch(const std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<";"<<__LINE__<<":\n"<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }

    co_return co_await retryForm.value();
}

Task<HttpResponsePtr> TicketController::destroy(HttpRequestPtr req, int32_t id) {
    SessionPtr session = getSession(req);

    if(!isLoggedIn(*session) ) // Not authenticated
        co_return HttpResponse::newNotFoundResponse();
    const Model::Ticket ticket = co_await mTicketOrm.findByPrimaryKey(id);
    const int32_t userId = getUserId(*session);
    if(!ticket.isReporter(userId) ) // Not authorised (i.e. only the defect reporter can delete their own ticket)
        co_return HttpResponse::newNotFoundResponse();

    co_await mTicketOrm.deleteByPrimaryKey(id);
    co_return HttpResponse::newRedirectionResponse("/", k303SeeOther);
}

Task<HttpResponsePtr> TicketController::throughNotification(HttpRequestPtr req, int32_t notificationId) {
    SessionPtr session = getSession(req);
    if(!isLoggedIn(*session) ) // Not authenticated
        co_return HttpResponse::newNotFoundResponse();

    try {
        const Model::Notification notification = co_await mNotifcationOrm.findByPrimaryKey(notificationId);

        if(notification.getValueOfUserId() != getUserId(*session) ) // Not authorised
            co_return HttpResponse::newNotFoundResponse();

        // Redirect to the notification's ticket page
        const std::string ticketId = std::to_string(notification.getValueOfTicketId() );
        co_await mNotifcationOrm.deleteByPrimaryKey(notificationId);
        co_return HttpResponse::newRedirectionResponse("/ticket/" + ticketId);
    } catch(const std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<";"<<__LINE__<<":\n"<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}


}  // namespace Ctrlr

