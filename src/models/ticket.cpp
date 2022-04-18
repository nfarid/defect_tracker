
#include "ticket.hpp"

#include "account.hpp"
#include "comment.hpp"
#include "notification.hpp"
#include "project.hpp"

#include "../util/core.hpp"
#include "../util/constants.hpp"
#include "../util/form_error.hpp"
#include "../util/string.hpp"
#include "../util/misc.hpp"

#include <drogon/HttpAppFramework.h>
#include <drogon/HttpViewData.h>

#include <vector>
#include <string>
#include <string_view>


namespace Model
{


using namespace drogon;
using namespace drogon::orm;
using namespace Util;
using std::string_literals::operator""s;


namespace
{


const std::vector<std::string_view> severityLst = {"low", "medium", "high"};
const std::vector<std::string_view> statusLst = {"new", "confirmed", "unreproducible", "resolved", "reopened"};


}  // namespace


drogon::Task<Ticket> Ticket::createTicket(const StringMap& postParams, const FileMap& fileParams,
        int32_t reporterId, int32_t projectId)
{
    // Obtain the data from the POST request
    const std::string title = getTrimmed(postParams.at("form-title") );
    const std::string description = getTrimmed(postParams.at("form-description") );
    const std::string severity = getTrimmed(postParams.at("form-severity") );
    std::string imageFilename = "";

    // TODO: Add more requirements for a valid username & password
    if(title.empty() )
        throw FormError("Title cannot be empty");
    if(title.size() > 80)
        throw FormError("Title cannot be longer than 80 characters");
    if(!std::ranges::all_of(title, [](char c){return c == ' ' || c == '-' || isAlNumUnderscore(c);}) )
        throw FormError("Title cannot contain invalid characters");
    if(description.empty() )
        throw FormError("Description cannot be empty");
    if(!contains(severityLst, severity) )
        throw FormError("Invalid severity.");

    // Handling the image upload
    const HttpFile& image = fileParams.at("form-image");
    if(image.getFileType() == FileType::FT_IMAGE) {  // If an image file is uploaded
        imageFilename = postParams.at("form-image");
        if(image.saveAs(imageFilename) != 0) // saveAs function returns zero on success
            throw FormError("Unable to upload image");
    } else if(image.getFileType() != 0) {  // If a non-image file is uploaded
        throw FormError("Non-image file has been uploaded");
    }

    // Validation is complete, so create a new ticket
    CoroMapper<Ticket> ticketOrm = app().getDbClient("db");
    Ticket newTicket;
    newTicket.setTitle(title);
    newTicket.setDescription(description);
    newTicket.setSeverity(severity);
    newTicket.setStatus("new");
    newTicket.setCreatedDate(trantor::Date::now() );
    newTicket.setReporterId(reporterId);
    newTicket.setProjectId(projectId);
    if(!imageFilename.empty() )
        newTicket.setImageFilename(std::move(imageFilename) );
    const Ticket createdTicket = co_await ticketOrm.insert(newTicket);

    // Create a notification for the new ticket
    CoroMapper<Notification> notificationOrm = app().getDbClient("db");
    Notification newNotifcation;
    newNotifcation.setSummary("A new defect has been reported: "s + title);
    newNotifcation.setTicketId( createdTicket.getValueOfId() );
    const Project project = co_await createdTicket.getProject();
    newNotifcation.setUserId( project.getValueOfManagerId() );  // notify the project manager when a ticket is created
    co_await notificationOrm.insert(newNotifcation);

    co_return newTicket;
}

Json::Value Ticket::getSeverityLst() {
    // Note: initialising static variables is thread safe
    const static Json::Value severityLstJson = Util::toJson(severityLst);
    return severityLstJson;
}

Json::Value Ticket::getStatusLst() {
    // Note: initialising static variables is thread safe
    const static Json::Value statusLstJson = Util::toJson(statusLst);
    return statusLstJson;
}

Task<Account> Ticket::getReporter() const {
    CoroMapper<Account> accountOrm = app().getDbClient("db");
    co_return co_await accountOrm.findByPrimaryKey(getValueOfReporterId() );
}

drogon::Task<Project> Ticket::getProject() const {
    CoroMapper<Project> projectOrm = app().getDbClient("db");
    co_return co_await projectOrm.findByPrimaryKey(getValueOfProjectId() );
}

drogon::Task<std::vector<Comment> > Ticket::getComments() const {
    CoroMapper<Comment> commentOrm = app().getDbClient("db");
    const Criteria belongToTicket{Comment::Cols::_ticket_id, CompareOperator::EQ, getValueOfId()};
    co_return co_await commentOrm.findBy(belongToTicket);
}

drogon::Task<bool> Ticket::canEdit(int32_t userId) const {
    const DbClientPtr db = app().getDbClient("db");

    // The reporter can edit the ticket
    if(isReporter(userId) )
        co_return true;

    // The staff can edit the ticket
    const Model::Project project = co_await getProject();
    co_return co_await project.isStaff(userId);
}

bool Ticket::isReporter(int32_t userId) const {
    return userId == getValueOfReporterId();
}

drogon::Task<std::vector<Account> > Ticket::getAssignables(int32_t userId) const {
    const Model::Project project = co_await getProject();

    // Manager can assign to any staff
    if(project.getValueOfManagerId() == userId)
        co_return co_await project.getStaff();

    // If the ticket is not yet assigned, then a staff can self-assign
    if(!getAssignedId() && co_await project.isStaff(userId) ) {
        CoroMapper<Account> accountOrm = app().getDbClient("db");
        const Account staff = co_await accountOrm.findByPrimaryKey(userId);
        co_return{staff};
    }

    // Else the user can assign to no one (i.e. they cannot assign)
    co_return{};
}

drogon::Task<> Ticket::update(const StringMap& postParams, int32_t userId) {
    CoroMapper<Ticket> ticketOrm = app().getDbClient("db");
    const Model::Project project  = co_await getProject();

    if(!co_await canEdit(userId) ) // cannot edit if not authorised
        throw FormError("Unauthoried to edit ticket");

    // The ticket's reporter can edit the ticket
    if(isReporter(userId) ) {
        const std::string description = getTrimmed(postParams.at("form-description") );
        setDescription(description);
    }

    // Can only assign to people in the assingableLst
    const std::vector assingableLst = co_await getAssignables(userId);
    if(!assingableLst.empty() && postParams.contains("form-assign") ) {
        const std::string assignedIdStr = getTrimmed(postParams.at("form-assign") );
        const int32_t assignedId = StrToNum{assignedIdStr};
        for(const auto& staff : assingableLst) {
            if(staff.getValueOfId() == assignedId) {  // if the assigned staff is valid
                if(!getAssignedId() || (getValueOfAssignedId() != assignedId) ) {  // if assigning to a new staff member
                    setAssignedId(assignedId);
                    // Create a notification
                    CoroMapper<Notification> notificationOrm = app().getDbClient("db");
                    Notification newNotifcation;
                    newNotifcation.setSummary("You have been assigned a ticket: "s + getValueOfTitle() );
                    newNotifcation.setTicketId(getValueOfId() );
                    newNotifcation.setUserId(assignedId);  // notify the assigned staff when assigning
                    co_await notificationOrm.insert(newNotifcation);
                }
            }
        }
    }

    // Staff can edit severity and status
    const bool isStaff = co_await project.isStaff(userId);
    if(isStaff) {
        const std::string severity = getTrimmed(postParams.at("form-severity") );
        const std::string status = getTrimmed(postParams.at("form-status") );
        if(!contains(severityLst, severity) )
            throw FormError("Severity is invalid");
        if(!contains(statusLst, status) )
            throw FormError("Status is invalid");
        setSeverity(severity);
        if( (status == "resolved") && (getValueOfStatus() != status) ) {  // if change status to resolved
            // Create a notification
            CoroMapper<Notification> notificationOrm = app().getDbClient("db");
            Notification newNotifcation;
            newNotifcation.setSummary("Your ticket has been resolved: "s + getValueOfTitle() );
            newNotifcation.setTicketId(getValueOfId() );
            newNotifcation.setUserId(getValueOfReporterId() );  // notify the original reporter after resolution
            co_await notificationOrm.insert(newNotifcation);
        }
        setStatus(status);
    }

    co_await ticketOrm.update(*this);
    co_return;
}

Json::Value Ticket::toViewJson() const {
    Json::Value json{};
    json["id"] = getValueOfId();
    json["title"] = HttpViewData::htmlTranslate(getValueOfTitle() );
    json["description"] = HttpViewData::htmlTranslate(getValueOfDescription() );
    json["status"] = HttpViewData::htmlTranslate(getValueOfStatus() );
    json["severity"] = HttpViewData::htmlTranslate(getValueOfSeverity() );
    const auto createdDate = getValueOfCreatedDate().toCustomedFormattedString(dateFormat);
    json["created-date"] = HttpViewData::htmlTranslate(createdDate);
    if(getResolvedDate() ) {
        const auto resolvedDate = getValueOfResolvedDate().toCustomedFormattedString(dateFormat);
        json["resolved-date"] = HttpViewData::htmlTranslate(resolvedDate);
    }
    if(getImageFilename() )
        json["image-filename"] = HttpViewData::htmlTranslate(getValueOfImageFilename() );
    json["reporter-id"] = getValueOfReporterId();
    if(getAssignedId() )
        json["assigned-id"] = getValueOfAssignedId();
    json["project-id"] = getValueOfProjectId();
    return json;
}


}  // namespace Model

