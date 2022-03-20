
#include "./auxiliary.hpp"
#include "../constants.hpp"
#include "../models/auxiliary.hpp"
#include "../models/comment.hpp"
#include "../models/project.hpp"
#include "../models/staff.hpp"
#include "../models/ticket.hpp"

#include <drogon/HttpController.h>

#include <algorithm>


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
        ADD_METHOD_TO(Ticket::newForm, "/project/{1}/report", Get);
        ADD_METHOD_TO(Ticket::edit, "/ticket/{1}/edit", Get);
        ADD_METHOD_TO(Ticket::create, "/project/{1}/report", Post);
        ADD_METHOD_TO(Ticket::update, "/ticket/{1}/edit", Post);
    METHOD_LIST_END
    /*YES-FORMAT*/

    void show(const HttpRequestPtr& req, ResponseCallback&& cb, int32_t id);
    void newForm(const HttpRequestPtr& req, ResponseCallback&& cb, int32_t projectId);
    void edit(const HttpRequestPtr& req, ResponseCallback&& cb, int32_t id);
    void create(const HttpRequestPtr& req, ResponseCallback&& cb, int32_t projectId);
    void update(const HttpRequestPtr& req, ResponseCallback&& cb, int32_t id);

private:
    Mapper<Model::Project> mProjectOrm = Mapper<Model::Project>(app().getDbClient("db") );
    Mapper<Model::Staff> mStaffOrm  = Mapper<Model::Staff>(app().getDbClient("db") );
    Mapper<Model::Ticket> mTicketOrm = Mapper<Model::Ticket>(app().getDbClient("db") );
    Mapper<Model::Comment> mCommentOrm = Mapper<Model::Comment>(app().getDbClient("db") );
};

void Ticket::show(const HttpRequestPtr& req, ResponseCallback&& cb, int32_t id) {
    try {
        const Model::Ticket ticket = mTicketOrm.findByPrimaryKey(id);
        const Model::Project project = mProjectOrm.findByPrimaryKey(ticket.getValueOfProjectId() );

        // Get a json list of comments
        Json::Value commentLstJson;
        for(const auto& comment : getTicketComments(id, mCommentOrm) )
            commentLstJson.append(comment.toJson() );

        // Send the data to the ticket view
        auto data = getViewData(ticket.getValueOfTitle(), *getSession(req) );
        data.insert("ticket", ticket.toJson() );
        data.insert("comment_lst", std::move(commentLstJson) );
        data.insert("project_name", project.getValueOfTitle() );
        return cb(HttpResponse::newHttpViewResponse("ticket.csp", data) );
    } catch(const std::exception& ex) {
        std::cerr<<ex.what()<<std::endl;
        return cb(HttpResponse::newNotFoundResponse() );
    }
}

void Ticket::newForm(const HttpRequestPtr& req, ResponseCallback&& cb, int32_t projectId) {
    try {
        auto projectFuture = mProjectOrm.findFutureByPrimaryKey(projectId);

        auto data = getViewData("New Ticket", *getSession(req) );
        const auto project = projectFuture.get();
        data.insert("project", project.toJson() );
        data.insert("severity_lst", severityLstJson() );

        return cb(HttpResponse::newHttpViewResponse("ticket_form.csp", data) );
    }  catch(const std::exception& ex) {
        std::cerr<<ex.what()<<std::endl;
        return cb(HttpResponse::newNotFoundResponse() );
    }
}

void Ticket::edit(const HttpRequestPtr& req, ResponseCallback&& cb, int32_t id) {
    try {
        const auto session = getSession(req);
        if(!isLoggedIn(*session) )
            return cb(HttpResponse::newNotFoundResponse() );

        auto ticketFuture = mTicketOrm.findFutureByPrimaryKey(id);
        auto data = getViewData("Edit Ticket", *session);
        data.insert("ticket_id", std::to_string(id) );

        const auto ticket = ticketFuture.get();
        auto projectFuture = mProjectOrm.findFutureByPrimaryKey(ticket.getValueOfProjectId() );
        data.insert("ticket_title", ticket.getValueOfTitle() );
        data.insert("ticket_description", ticket.getValueOfDescription() );
        const bool isAssigned = !!ticket.getAssignedId();
        if(isAssigned)
            data.insert("ticket_assigned", std::to_string(ticket.getValueOfAssignedId() ) );

        bool authorised = false;
        const auto userId = session->get<int32_t>("user_id");
        // If the user was the original reporter, then they can edit the ticket
        if(userId == ticket.getValueOfReporterId() ) {
            authorised = true;
            data.insert("can_edit", true);
        }

        const auto project = projectFuture.get();
        const Criteria staffCriteria{Model::Staff::Cols::_project_id, CompareOperator::EQ, project.getValueOfId()};
        const auto staffLst = mStaffOrm.findBy(staffCriteria);
        // If the user was project manager, then they can assign to a staff member
        if(userId == project.getValueOfManagerId() ) {
            authorised = true;
            data.insert("can_assign", true);

            Json::Value staffLstJson;
            for(const auto& staff : staffLst)
                staffLstJson.append(staff.toJson() );
            data.insert("staff_lst", staffLstJson);
        }         // TODO: Allow staff to self assign

        // If the user is not authorised to edit any parts of the ticket
        if(!authorised)
            return cb(HttpResponse::newNotFoundResponse() );

        return cb(HttpResponse::newHttpViewResponse("ticket_edit.csp", data) );
    } catch(const std::exception& ex) {
        std::cerr<<ex.what()<<std::endl;
        return cb(HttpResponse::newNotFoundResponse() );
    }
}

void Ticket::create(const HttpRequestPtr& req, ResponseCallback&& cb, int32_t projectId) {
    try {
        const auto& postParams = req->parameters();
        const Model::Project project = mProjectOrm.findByPrimaryKey(projectId);
        HttpViewData data = getViewData("New Ticket", *getSession(req) );
        data.insert("severity_lst", severityLstJson() );
        data.insert("project", project.toJson() );

        // Check if the form data has been entered
        // TODO: Refactor this
        const auto titleIter = postParams.find("form-title");
        if(titleIter == end(postParams) ) {
            data.insert("form_error", "Title has not been entered"s);
            auto resp = HttpResponse::newHttpViewResponse("ticket_form.csp", data);
            resp->setStatusCode(k422UnprocessableEntity);
            return cb(resp);
        }
        const std::string& title = titleIter->second;
        data.insert("form_title", title);
        const auto descriptionIter = postParams.find("form-description");
        if(descriptionIter == end(postParams) ) {
            data.insert("form_error", "Description has not been entered"s);
            auto resp = HttpResponse::newHttpViewResponse("ticket_form.csp", data);
            resp->setStatusCode(k422UnprocessableEntity);
            return cb(resp);
        }
        const std::string& description = descriptionIter->second;
        data.insert("form_description", description);
        const auto severityIter = postParams.find("form-severity");
        if(descriptionIter == end(postParams) ) {
            data.insert("form_error", "Severity has not been entered"s);
            auto resp = HttpResponse::newHttpViewResponse("ticket_form.csp", data);
            resp->setStatusCode(k422UnprocessableEntity);
            return cb(resp);
        }
        const std::string& severity = severityIter->second;

        // Verify form data
        // TODO: Add more checks
        // Check if severity is valid (i.e it's in severityLst)
        if(std::find(begin(severityLst), end(severityLst), severity) == end(severityLst) ) {
            data.insert("form_error", "Severity has not been entered correctly"s);
            auto resp = HttpResponse::newHttpViewResponse("ticket_form.csp", data);
            resp->setStatusCode(k422UnprocessableEntity);
            return cb(resp);
        }

        // Insert the new ticket into the database
        Model::Ticket newTicket;
        newTicket.setTitle(title);
        newTicket.setDescription(description);
        newTicket.setSeverity(severity);
        newTicket.setStatus("new");
        newTicket.setCreatedDate(trantor::Date::now() );
        newTicket.setProjectId(projectId);
        mTicketOrm.insert(newTicket);
        return cb(HttpResponse::newRedirectionResponse("/", k303SeeOther) );
    }  catch(const std::exception& ex) {
        std::cerr<<ex.what()<<std::endl;
        return cb(HttpResponse::newNotFoundResponse() );
    }
}

void Ticket::update(const HttpRequestPtr& req, ResponseCallback&& cb, int32_t id) {
    try {
        std::future ticketFuture = mTicketOrm.findFutureByPrimaryKey(id);
        const auto session = req->session();
        const auto& postParams = req->parameters();
        Model::Ticket ticket = ticketFuture.get();

        if(!isLoggedIn(*session) )
            return cb(HttpResponse::newNotFoundResponse() );

        const int32_t userId = session->get<int32_t>("user_id");
        // The ticket's reporter can edit the ticket
        if(userId == ticket.getValueOfReporterId() ) {
            ticket.setDescription(postParams.at("ticketDescription") );
            mTicketOrm.update(ticket);
        }

        // TODO: Manager can assign

        return cb(HttpResponse::newRedirectionResponse("/") );
    }  catch(const std::exception& ex) {
        std::cerr<<ex.what()<<std::endl;
        return cb(HttpResponse::newNotFoundResponse() );
    }
}


}  // namespace Ctrlr

