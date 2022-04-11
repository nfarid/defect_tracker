
#include "ticket.hpp"

#include "account.hpp"
#include "comment.hpp"
#include "project.hpp"

#include "../util/core.hpp"
#include "../util/constants.hpp"
#include "../util/form_error.hpp"
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


UTIL_INTERNAL std::vector<std::string> severityLst = {"low", "medium", "high"};
UTIL_INTERNAL std::vector<std::string> statusLst = {"new", "confirmed", "unreproducible", "resolved", "reopened"};

UTIL_INTERNAL Json::Value lstToJson(const std::vector<std::string>& lst) {
    Json::Value json{};
    for(const auto& elem : lst)
        json.append(elem);
    return json;
}

template<typename T, typename C>
UTIL_INTERNAL bool contains(const C& container, const T& value) {
    return std::find(cbegin(container), cend(container), value) != cend(container);
}

drogon::Task<Ticket> Ticket::createTicket(const Util::StringMap& postParams, const Util::FileMap& fileParams,
        int32_t reporterId, int32_t projectId)
{
    CoroMapper<Ticket> orm = app().getDbClient("db");

    // Obtain the data from the POST request
    const std::string title = postParams.at("form-title");
    const std::string description = postParams.at("form-description");
    const std::string severity = postParams.at("form-severity");
    std::string imageFilename = "";

    // TODO: Add more requirements for a valid username & password
    if(title.empty() )
        throw Util::FormError("Title cannot be empty");
    if(description.empty() )
        throw Util::FormError("Description cannot be empty");
    if(!contains(severityLst, severity) )
        throw Util::FormError("Invalid severity.");

    // Handling the image upload
    const HttpFile& image = fileParams.at("form-image");
    if(image.getFileType() == FileType::FT_IMAGE) {  // If an image file is uploaded
        imageFilename = postParams.at("form-image");
        if(image.saveAs(imageFilename) != 0) // saveAs function returns zero on success
            throw Util::FormError("Unable to upload image");
    } else if(image.getFileType() != 0) {  // If a non-image file is uploaded
        throw Util::FormError("Non-image file has been uploaded");
    }

    // Validation is complete, so create a new ticket
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

    co_return co_await orm.insert(newTicket);
}

Json::Value Ticket::getSeverityLst() {
    // Note: initialising static variables is thread safe
    const static Json::Value severityLstJson = lstToJson(severityLst);
    return severityLstJson;
}

Json::Value Ticket::getStatusLst() {
    // Note: initialising static variables is thread safe
    const static Json::Value statusLstJson = lstToJson(statusLst);
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

drogon::Task<> Ticket::update(const Util::StringMap& postParams, int32_t userId) {
    // TODO: html readonly input

    if(!co_await canEdit(userId) ) // cannot edit if not authorised
        throw Util::FormError("Unauthoried to edit ticket");

    // The ticket's reporter can edit the ticket
    if(isReporter(userId) )
        setDescription(postParams.at("form-description") );

    // Can only assign to people in the assingableLst
    const std::vector assingableLst = co_await getAssignables(userId);
    if(!assingableLst.empty() && postParams.contains("form-assign") ) {
        const int32_t assignedId = Util::StrToNum{postParams.at("form-assign")};
        for(const auto& staff : assingableLst) {
            if(staff.getValueOfId() == assignedId)
                setAssignedId(assignedId);
        }
    }

    // Staff can edit severity and status
    const Model::Project project  = co_await getProject();
    const bool isStaff = co_await project.isStaff(userId);
    if(isStaff) {
        const std::string severity = postParams.at("form-severity");
        const std::string status = postParams.at("form-status");
        if(!contains(severityLst, severity) )
            throw Util::FormError("Severity is invalid");
        if(!contains(statusLst, status) )
            throw Util::FormError("Status is invalid");
        setSeverity(severity);
        setStatus(status);
    }

    CoroMapper<Ticket> orm = app().getDbClient("db");
    co_await orm.update(*this);
    co_return;
}

Json::Value Ticket::toViewJson() const {
    Json::Value json{};
    json["id"] = getValueOfId();
    json["title"] = HttpViewData::htmlTranslate(getValueOfTitle() );
    json["description"] = HttpViewData::htmlTranslate(getValueOfDescription() );
    json["status"] = HttpViewData::htmlTranslate(getValueOfStatus() );
    json["severity"] = HttpViewData::htmlTranslate(getValueOfSeverity() );
    const auto createdDate = getValueOfCreatedDate().toCustomedFormattedString(Util::dateFormat);
    json["created-date"] = HttpViewData::htmlTranslate(createdDate);
    if(getResolvedDate() ) {
        const auto resolvedDate = getValueOfResolvedDate().toCustomedFormattedString(Util::dateFormat);
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

