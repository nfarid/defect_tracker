
#include "auxiliary.hpp"
#include "../models/account.hpp"
#include "../models/project.hpp"
#include "../models/ticket.hpp"
#include "../util/form_error.hpp"

#include <drogon/HttpController.h>

#include <algorithm>
#include <string>


namespace Ctrlr
{


using namespace Aux;
using namespace drogon;
using namespace drogon::orm;
using std::string_literals::operator""s;
using namespace std::chrono;

class ProjectController : public HttpController<ProjectController> {
public:
    /*NO-FORMAT*/
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(ProjectController::show, "/project/{1}", Get);
        ADD_METHOD_TO(ProjectController::newForm, "/project-new", Get);
        ADD_METHOD_TO(ProjectController::create, "/project-new", Post);
        ADD_METHOD_TO(ProjectController::search, "/search", Get);
        ADD_METHOD_TO(ProjectController::destroy, "/project/{1}/delete", Post);
    METHOD_LIST_END
    /*YES-FORMAT*/

    Task<HttpResponsePtr> show(HttpRequestPtr req, int32_t id);
    Task<HttpResponsePtr> newForm(HttpRequestPtr req);
    Task<HttpResponsePtr> create(HttpRequestPtr req);
    Task<HttpResponsePtr> search(HttpRequestPtr req);
    Task<HttpResponsePtr> destroy(HttpRequestPtr req, int32_t id);

private:
    CoroMapper<Model::Project> mProjectOrm = app().getDbClient("db");

    HttpResponsePtr newImpl(HttpRequestPtr req, Util::StringMap formData,
            std::string errorMessage);

    Json::Value getStats(const std::vector<Model::Ticket>& ticketLst);
};

Task<HttpResponsePtr> ProjectController::show(HttpRequestPtr req, int32_t id)
{
    try {
        const Model::Project project = co_await mProjectOrm.findByPrimaryKey(id);
        const Model::Account manager = co_await project.getManager();
        const std::vector ticketLst = co_await project.getTickets();

        HttpViewData data = getViewData(project.getValueOfTitle(), *getSession(req) );
        data.insert("project", project.toViewJson() );
        data.insert("manager-name", manager.getValueOfUsername() );
        data.insert("ticket-lst", toViewJson(ticketLst) );
        data.insert("severity-lst", Model::Ticket::getSeverityLst() );
        data.insert("status-lst", Model::Ticket::getStatusLst() );
        data.insert("stats", getStats(ticketLst) );
        co_return HttpResponse::newHttpViewResponse("project_show.csp", data);
    } catch(std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}

Json::Value ProjectController::getStats(const std::vector<Model::Ticket>& ticketLst) {
    Json::Value stats{};
    stats["ticket-count"] = static_cast<Json::UInt64>(size(ticketLst) );

    // Obtain average resolution time
    microseconds totalResolutionDuration{0};
    int countResolution = 0;
    for(const auto& ticket : ticketLst) {
        if(ticket.getResolvedDate() ) {  // check if the ticket has resolved date - i.e. its already resolved
            const int64_t resolvedEpoch = ticket.getValueOfResolvedDate().microSecondsSinceEpoch();
            const int64_t creationEpoch = ticket.getValueOfCreatedDate().microSecondsSinceEpoch();
            const microseconds resolutionDuration{resolvedEpoch - creationEpoch};
            totalResolutionDuration += resolutionDuration;
            ++countResolution;
        }
    }
    if(countResolution != 0) {
        const microseconds averageResolutionDuration = totalResolutionDuration / countResolution;
        const hours averageResolutionDurationHours = duration_cast<hours>(averageResolutionDuration);
        stats["average-resolution-duration-hours"] = static_cast<Json::Int64>(averageResolutionDurationHours.count() );
    }

    // Obtain count of types of tickets:
    for(const auto& statusType : Model::Ticket::getStatusLst() )
        stats[statusType.asString()] = 0;
    for(const auto& severity : Model::Ticket::getSeverityLst() )
        stats[severity.asString()] = 0;

    for(const auto& ticket : ticketLst) {
        stats[ticket.getValueOfStatus()] = stats[ticket.getValueOfStatus()].asInt() + 1;
        stats[ticket.getValueOfSeverity()] = stats[ticket.getValueOfSeverity()].asInt() + 1;
    }

    return stats;
}

Task<HttpResponsePtr> ProjectController::newForm(HttpRequestPtr req){
    co_return newImpl(req, {}, "");
}

HttpResponsePtr ProjectController::newImpl(HttpRequestPtr req,
        Util::StringMap formData, std::string errorMessage)
{
    const SessionPtr session = getSession(req);
    // If the user hasn't logged in, they cannot create a project
    if(!isLoggedIn(*session) )
        return HttpResponse::newRedirectionResponse("/");

    // Else show the project creation
    HttpViewData data = getViewData("Create project."s, *session);
    data.insert("form-action", "/project-new"s);

    // If there was an error, then redisplay the form data
    data.insert("form-error", errorMessage);
    for(const auto& [k, v] : formData)
        data.insert(k, HttpViewData::htmlTranslate(v) );

    return HttpResponse::newHttpViewResponse("project_new.csp", data);
}

Task<HttpResponsePtr> ProjectController::create(HttpRequestPtr req) {
    const SessionPtr session = getSession(req);
    // If the user hasn't logged in, they cannot create a project
    if(!isLoggedIn(*session) )
        co_return HttpResponse::newRedirectionResponse("/");
    const int32_t userId = getUserId(*session);

    // Data from the HTTP POST request
    const auto& postParams = req->parameters();

    try {
        if(!isValidToken(postParams.at("token"), *session) ) // token must be valid
            throw std::runtime_error("Invalid token");

        const Model::Project project = co_await Model::Project::createProject(postParams, userId);
        co_return HttpResponse::newRedirectionResponse("/", k303SeeOther);
    } catch(Util::FormError& ex) {
        // There was a form error, so let the user retry again
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<ex.what()<<std::endl;
        co_return newImpl(req, postParams, ex.what() );
    }  catch(const std::exception& ex) {
        // An unexpected error has occured
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}

Task<HttpResponsePtr> ProjectController::search(HttpRequestPtr req){
    const std::string_view param = req->parameters().at("search-project");
    try{
        const std::vector projectLst = co_await Model::Project::searchProject(param);
        HttpViewData data = getViewData("project_search", *getSession(req) );
        data.insert("project-lst", toViewJson(projectLst) );
        co_return HttpResponse::newHttpViewResponse("project_search.csp", data);
    } catch(const std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}

Task<HttpResponsePtr> ProjectController::destroy(HttpRequestPtr req, int32_t id) {
    SessionPtr session = getSession(req);
    const Util::StringMap& postParams = req->parameters();

    if(!isLoggedIn(*session) ) // Not authenticated
        co_return HttpResponse::newNotFoundResponse();
    if(!isValidToken(postParams.at("token"), *session) ) {  // Token isn't valid
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<"Invalid token"<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }

    const Model::Project project = co_await mProjectOrm.findByPrimaryKey(id);

    if(getUserId(*session) != project.getValueOfManagerId() ) // Not authorised (i.e. only project manager can delete their own project)
        co_return HttpResponse::newNotFoundResponse();

    co_await mProjectOrm.deleteByPrimaryKey(id);
    co_return HttpResponse::newRedirectionResponse("/", k303SeeOther);
}


}// namespace Ctrlr

