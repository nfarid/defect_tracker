
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
        ADD_METHOD_TO(TicketController::showTicket, "/ticket/{1}", Get);
        ADD_METHOD_TO(TicketController::newForm, "/project/{1}/report", Get, "Fltr::OnlyLoggedIn");
        ADD_METHOD_TO(TicketController::editForm, "/ticket/{1}/edit", Get, "Fltr::OnlyLoggedIn");
        ADD_METHOD_TO(TicketController::createSubmit, "/project/{1}/report", Post, "Fltr::OnlyLoggedIn", "Fltr::ValidToken");
        ADD_METHOD_TO(TicketController::updateSubmit, "/ticket/{1}/edit", Post, "Fltr::OnlyLoggedIn", "Fltr::ValidToken");
        ADD_METHOD_TO(TicketController::destroy, "/ticket/{1}/delete", Post, "Fltr::OnlyLoggedIn", "Fltr::ValidToken");
        ADD_METHOD_TO(TicketController::throughNotification, "/notification/{1}", Post, "Fltr::OnlyLoggedIn");
    METHOD_LIST_END
    /*YES-FORMAT*/

    // showTicket is called when the user accesses the ticket's page
    // For example if the path is /ticket/80 then ticketId would be 80
    Task<HttpResponsePtr> showTicket(HttpRequestPtr req, IdType ticketId);
    // throughNotification is called when a user clicks on notification for a ticket
    Task<HttpResponsePtr> throughNotification(HttpRequestPtr req, IdType notificationId);

    // newForm is called when the user acceses the ticket's creation form
    Task<HttpResponsePtr> newForm(HttpRequestPtr req, IdType projectId);
    // createSubmit is called when the user submits the ticket's creation form
    Task<HttpResponsePtr> createSubmit(HttpRequestPtr req, IdType projectId);
    // destroy is called when the ticket's reporter wants to delete the ticket
    Task<HttpResponsePtr> destroy(HttpRequestPtr req, IdType ticketId);

    // editForm is called when the user wants to edit the ticket
    Task<HttpResponsePtr> editForm(HttpRequestPtr req, IdType ticketId);
    // updateSubmit is called when the user sumbits the edit's form
    Task<HttpResponsePtr> updateSubmit(HttpRequestPtr req, IdType ticketId);

private:
    Task<HttpResponsePtr> newFormImpl(HttpRequestPtr req, IdType projectId,
            const Util::StringMap& formData, const std::string& errorMessage);
};

Task<HttpResponsePtr> TicketController::showTicket(HttpRequestPtr req, IdType ticketId) {
    const SessionPtr session = getSession(req);

    try {
        // Obtain the ticket from the database
        const Ticket ticket = co_await Ticket::findByPrimaryKey(ticketId);
        const auto reporterAwaiter = ticket.getReporter();
        const auto projectAwaiter = ticket.getProject();
        const auto commentLstAwaiter = ticket.getComments();
        const Account reporter = co_await reporterAwaiter;
        const Project project = co_await projectAwaiter;
        const std::vector commentLst = co_await commentLstAwaiter;

        // Add the ticket's information to the view
        HttpViewData viewData = getViewData(ticket.getValueOfTitle(), *session);
        viewData.insert("ticket", ticket.toViewJson() );
        viewData.insert("project", project.toViewJson() );
        viewData.insert("reporter", reporter.toViewJson() );
        viewData.insert("comment-lst", toViewJson(commentLst) );
        if(isLoggedIn(*session) ) {
            const IdType userId = getUserId(*session);
            viewData.insert("can-edit", co_await ticket.canEdit(userId) );
            viewData.insert("can-delete", ticket.isReporter(userId) );
        }
        // Display the ticket's show page
        co_return HttpResponse::newHttpViewResponse("ticket_show.csp", viewData);
    } catch(const std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<typeid(ex).name()<<" ; "<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}

Task<HttpResponsePtr> TicketController::newForm(HttpRequestPtr req, IdType projectId) {
    co_return co_await newFormImpl(req, projectId, {}, "");
}

Task<HttpResponsePtr> TicketController::newFormImpl(HttpRequestPtr req, IdType projectId,
        const Util::StringMap& formData, const std::string& errorMessage)
{
    const SessionPtr session = getSession(req);
    try {
        const Model::Project project = co_await Project::findByPrimaryKey(projectId);

        HttpViewData data = getViewData("New Ticket", *session);
        data.insert("project", project.toViewJson() );
        data.insert("severity-lst", Model::Ticket::getSeverityLst() );

        // If there was an error from a previous form attempt, then redisplay the form data
        data.insert("form-error", errorMessage);
        for(const auto& [k, v] : formData)
            data.insert(k, HttpViewData::htmlTranslate(v) );

        co_return HttpResponse::newHttpViewResponse("ticket_new.csp", data);
    }  catch(const std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<typeid(ex).name()<<" ; "<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}

Task<HttpResponsePtr> TicketController::createSubmit(HttpRequestPtr req, IdType projectId) {
    const SessionPtr session = getSession(req);
    const IdType userId = getUserId(*session);
    const auto& [postParams, fileParams] = parseMultiPart(req);

    // Using std::optional to defer initialisation
    std::optional<Task<HttpResponsePtr> > retryForm;

    try {
        const Ticket createdTicket = co_await Ticket::createTicket(postParams, fileParams, userId, projectId);
        const std::string createdTicketId = std::to_string(createdTicket.getValueOfId() );
        co_return HttpResponse::newRedirectionResponse("/ticket/" + createdTicketId);
    }  catch(const Util::FormError& ex) {
        // If there's a form error, then retry the ticket creation form
        // co_await cannot be done in catch handler
        retryForm = newFormImpl(req, projectId, postParams, ex.what() );
    } catch(const std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<typeid(ex).name()<<" ; "<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
    co_return co_await retryForm.value();
}

Task<HttpResponsePtr> TicketController::editForm(HttpRequestPtr req, IdType ticketId) {
    const auto session = getSession(req);
    const IdType userId = getUserId(*session);

    try {
        const Model::Ticket ticket = co_await Ticket::findByPrimaryKey(ticketId);
        const Model::Project project = co_await ticket.getProject();
        if( !co_await ticket.canEdit(userId) ) // cannot edit if not authorised
            co_return HttpResponse::newRedirectionResponse("/");

        // Add the data to the view
        HttpViewData viewData = getViewData("Edit Ticket", *session);
        viewData.insert("ticket", ticket.toViewJson() );
        viewData.insert("assignable_lst",  toViewJson(co_await ticket.getAssignables(userId) ) );
        viewData.insert("is_reporter", ticket.isReporter(userId) );
        viewData.insert("is_staff", co_await project.isStaff(userId) );
        viewData.insert("status_lst", Model::Ticket::getStatusLst() );
        viewData.insert("severity-lst", Model::Ticket::getSeverityLst() );

        co_return HttpResponse::newHttpViewResponse("ticket_edit.csp", viewData);
    } catch(const std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<typeid(ex).name()<<" ; "<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}

Task<HttpResponsePtr> TicketController::updateSubmit(HttpRequestPtr req, IdType ticketId) {
    const auto session = getSession(req);
    const IdType userId = getUserId(*session);

    const auto& postParams = req->parameters();
    std::optional<Task<HttpResponsePtr> > retryForm;
    try {
        Model::Ticket ticket = co_await Ticket::findByPrimaryKey(ticketId);
        co_await ticket.update(postParams, userId);
        co_return HttpResponse::newRedirectionResponse("/ticket/" + std::to_string(ticketId) );
    } catch(const Util::FormError& ex) {
        retryForm = editForm(req, ticketId);  // If there's a form error, then retry
    }  catch(const std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<typeid(ex).name()<<" ; "<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }

    co_return co_await retryForm.value();
}

Task<HttpResponsePtr> TicketController::destroy(HttpRequestPtr req, IdType ticketId) {
    SessionPtr session = getSession(req);

    const Model::Ticket ticket = co_await Ticket::findByPrimaryKey(ticketId);
    const IdType userId = getUserId(*session);
    if(!ticket.isReporter(userId) ) // Not authorised (i.e. only the defect reporter can delete their own ticket)
        co_return HttpResponse::newNotFoundResponse();

    co_await Ticket::deleteByPrimaryKey(ticketId);
    co_return HttpResponse::newRedirectionResponse("/");
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

