
#include "auxiliary.hpp"

#include "../models/account.hpp"
#include "../models/comment.hpp"
#include "../models/notification.hpp"
#include "../models/project.hpp"
#include "../models/staff.hpp"
#include "../models/ticket.hpp"

#include "../util/database.hpp"
#include "../util/form_error.hpp"
#include "../util/misc.hpp"

#include <drogon/HttpController.h>

#include <algorithm>


namespace Ctrlr
{


using namespace drogon;
using namespace drogon::orm;
using namespace Aux;
using namespace Model;
using std::string_literals::operator""s;


class TicketController : public HttpController<TicketController> {
public:
    /*NO-FORMAT*/
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(TicketController::show, "/ticket/{1}", Get);
        ADD_METHOD_TO(TicketController::newForm, "/project/{1}/report", Get, "Fltr::OnlyLoggedIn");
        ADD_METHOD_TO(TicketController::edit, "/ticket/{1}/edit", Get, "Fltr::OnlyLoggedIn");
        ADD_METHOD_TO(TicketController::create, "/project/{1}/report", Post, "Fltr::OnlyLoggedIn", "Fltr::ValidToken");
        ADD_METHOD_TO(TicketController::update, "/ticket/{1}/edit", Post, "Fltr::OnlyLoggedIn", "Fltr::ValidToken");
        ADD_METHOD_TO(TicketController::destroy, "/ticket/{1}/delete", Post, "Fltr::OnlyLoggedIn", "Fltr::ValidToken");
        ADD_METHOD_TO(TicketController::throughNotification, "/notification/{1}", Post, "Fltr::OnlyLoggedIn");
    METHOD_LIST_END
    /*YES-FORMAT*/

    Task<HttpResponsePtr> show(HttpRequestPtr req, IdType id);
    Task<HttpResponsePtr> newForm(HttpRequestPtr req, IdType projectId);
    Task<HttpResponsePtr> edit(HttpRequestPtr req, IdType id);
    Task<HttpResponsePtr> create(HttpRequestPtr req, IdType projectId);
    Task<HttpResponsePtr> update(HttpRequestPtr req, IdType id);
    Task<HttpResponsePtr> destroy(HttpRequestPtr req, IdType id);
    Task<HttpResponsePtr> throughNotification(HttpRequestPtr req, IdType notificationId);

private:
    Task<HttpResponsePtr> newImpl(HttpRequestPtr req, IdType projectId,
            Util::StringMap formData, std::string errorMessage);
};

Task<HttpResponsePtr> TicketController::show(HttpRequestPtr req, IdType id) {
    const SessionPtr session = getSession(req);

    try {
        const Model::Ticket ticket = co_await Ticket::findByPrimaryKey(id);
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
            const IdType userId = getUserId(*session);
            data.insert("can-edit", co_await ticket.canEdit(userId) );
            data.insert("can-delete", ticket.isReporter(userId) );
        }

        co_return HttpResponse::newHttpViewResponse("ticket_show.csp", data);
    } catch(const std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<";"<<__LINE__<<":\n"<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}

Task<HttpResponsePtr> TicketController::newForm(HttpRequestPtr req, IdType projectId) {
    return newImpl(req, projectId, {}, "");
}

Task<HttpResponsePtr> TicketController::newImpl(HttpRequestPtr req, IdType projectId,
        Util::StringMap formData, std::string errorMessage)
{
    const SessionPtr session = getSession(req);
    try {
        const Model::Project project = co_await Project::findByPrimaryKey(projectId);

        HttpViewData data = getViewData("New Ticket", *session);
        data.insert("project", project.toViewJson() );
        data.insert("severity-lst", Model::Ticket::getSeverityLst() );
        data.insert("form-error", errorMessage);
        for(const auto& [k, v] : formData)
            data.insert(k, HttpViewData::htmlTranslate(v) );

        co_return HttpResponse::newHttpViewResponse("ticket_new.csp", data);
    }  catch(const std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<";"<<__LINE__<<":\n"<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}

Task<HttpResponsePtr> TicketController::edit(HttpRequestPtr req, IdType id) {
    const auto session = getSession(req);
    const IdType userId = getUserId(*session);

    try {
        const Model::Ticket ticket = co_await Ticket::findByPrimaryKey(id);
        const Model::Project project = co_await ticket.getProject();
        if(!co_await ticket.canEdit(userId) ) // cannot edit if not authorised
            co_return HttpResponse::newRedirectionResponse("/");

        HttpViewData data = getViewData("Edit Ticket", *session);
        data.insert("ticket", ticket.toViewJson() );
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

Task<HttpResponsePtr> TicketController::create(HttpRequestPtr req, IdType projectId) {
    const SessionPtr session = getSession(req);
    const IdType userId = getUserId(*session);
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

Task<HttpResponsePtr> TicketController::update(HttpRequestPtr req, IdType id) {
    const auto session = getSession(req);
    const IdType userId = getUserId(*session);

    const auto& postParams = req->parameters();
    std::optional<Task<HttpResponsePtr> > retryForm;
    try {
        Model::Ticket ticket = co_await Ticket::findByPrimaryKey(id);
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

Task<HttpResponsePtr> TicketController::destroy(HttpRequestPtr req, IdType id) {
    SessionPtr session = getSession(req);

    const Model::Ticket ticket = co_await Ticket::findByPrimaryKey(id);
    const IdType userId = getUserId(*session);
    if(!ticket.isReporter(userId) ) // Not authorised (i.e. only the defect reporter can delete their own ticket)
        co_return HttpResponse::newNotFoundResponse();

    co_await Ticket::deleteByPrimaryKey(id);
    co_return HttpResponse::newRedirectionResponse("/", k303SeeOther);
}

Task<HttpResponsePtr> TicketController::throughNotification(HttpRequestPtr req, IdType notificationId) {
    SessionPtr session = getSession(req);
    try {
        const Notification notification = co_await Notification::findByPrimaryKey(notificationId);

        if(notification.getValueOfUserId() != getUserId(*session) ) // Not authorised
            co_return HttpResponse::newNotFoundResponse();

        // Redirect to the notification's ticket page
        const std::string ticketId = std::to_string(notification.getValueOfTicketId() );
        co_await Notification::deleteByPrimaryKey(notificationId);
        co_return HttpResponse::newRedirectionResponse("/ticket/" + ticketId);
    } catch(const std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<";"<<__LINE__<<":\n"<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}


}  // namespace Ctrlr

