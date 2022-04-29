
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

class ProjectController : public HttpController<ProjectController> {
public:
    /*NO-FORMAT*/
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(ProjectController::showProject, "/project/{1}", Get);
        ADD_METHOD_TO(ProjectController::newForm, "/project-new", Get, "Fltr::OnlyLoggedIn");
        ADD_METHOD_TO(ProjectController::createSubmit, "/project-new", Post, "Fltr::OnlyLoggedIn", "Fltr::ValidToken");
        ADD_METHOD_TO(ProjectController::search, "/search", Get);
        ADD_METHOD_TO(ProjectController::destroy, "/project/{1}/delete", Post, "Fltr::OnlyLoggedIn", "Fltr::ValidToken");
    METHOD_LIST_END
    /*YES-FORMAT*/

    // showProject is called when the user accesses the project's page (e.g. /project/23)
    // For example if the path is /project/23 then projectId would be 23
    Task<HttpResponsePtr> showProject(HttpRequestPtr req, IdType projectId);

    // newForm is called when the user is acceses the project's creation form
    Task<HttpResponsePtr> newForm(HttpRequestPtr req);

    // createSubmit is called when the user submits the project's creation form
    Task<HttpResponsePtr> createSubmit(HttpRequestPtr req);

    // search is called when a user uses the search query
    Task<HttpResponsePtr> search(HttpRequestPtr req);

    // destroy is called when a project manager wants to delete the project
    Task<HttpResponsePtr> destroy(HttpRequestPtr req, IdType projectId);

private:
    HttpResponsePtr newFormImpl(HttpRequestPtr req, const Util::StringMap& formData, const std::string& formError);
};

Task<HttpResponsePtr> ProjectController::showProject(HttpRequestPtr req, IdType projectId)
{
    const SessionPtr session = getSession(req);
    try {
        const Model::Project project = co_await Model::Project::findByPrimaryKey(projectId);
        const Model::Account manager = co_await project.getManager();
        const std::vector ticketLst = co_await project.getTickets();

        HttpViewData viewData = getViewData(project.getValueOfTitle(), *getSession(req) );
        viewData.insert("project", project.toViewJson() );
        viewData.insert("manager-name", manager.getValueOfUsername() );
        viewData.insert("ticket-lst", toViewJson(ticketLst) );
        viewData.insert("severity-lst", Model::Ticket::getSeverityLst() );
        viewData.insert("status-lst", Model::Ticket::getStatusLst() );
        viewData.insert("stats", Model::Ticket::getStatistics(ticketLst) );
        viewData.insert("is-manager", project.isManager(getUserId(*session) ) );
        co_return HttpResponse::newHttpViewResponse("project_show.csp", viewData);
    } catch(std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}

Task<HttpResponsePtr> ProjectController::newForm(HttpRequestPtr req){
    co_return newFormImpl(req, {}, "");
}

HttpResponsePtr ProjectController::newFormImpl(HttpRequestPtr req, const Util::StringMap& formData,
        const std::string& formError)
{
    HttpViewData viewData = getViewData("Create project."s, *getSession(req) );

    // If there was an error from a previous form attempt, then redisplay the form data
    viewData.insert("form-error", formError);
    for(const auto& [k, v] : formData)
        viewData.insert(k, HttpViewData::htmlTranslate(v) );

    return HttpResponse::newHttpViewResponse("project_new.csp", viewData);
}

Task<HttpResponsePtr> ProjectController::createSubmit(HttpRequestPtr req) {
    const SessionPtr session = getSession(req);
    const IdType userId = getUserId(*session);

    // Obtain the form data from the HTTP POST request
    const Util::StringMap& formData = req->parameters();

    try {
        const Model::Project project = co_await Model::Project::createProject(formData, userId);
        co_return HttpResponse::newRedirectionResponse("/");
    } catch(Util::FormError& ex) {
        // There was a form error, so let the user retry again
        co_return newFormImpl(req, formData, ex.what() );
    }  catch(const std::exception& ex) {
        // An unexpected error has occured
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}

Task<HttpResponsePtr> ProjectController::search(HttpRequestPtr req){
    try{
        const std::string_view getQuery = req->parameters().at("search-project");  // Get the search query
        const std::vector projectLst = co_await Model::Project::searchProject(getQuery);  // Obtain any suitable projects from the query
        HttpViewData viewData = getViewData("project_search", *getSession(req) );
        viewData.insert("project-lst", toViewJson(projectLst) );
        co_return HttpResponse::newHttpViewResponse("project_search.csp", viewData);
    } catch(const std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<std::endl;
        std::cerr<<typeid(ex).name()<<" ; "<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}

Task<HttpResponsePtr> ProjectController::destroy(HttpRequestPtr req, IdType projectId) {
    SessionPtr session = getSession(req);

    // Check if the user is the project manager
    const Model::Project project = co_await Model::Project::findByPrimaryKey(projectId);
    if(getUserId(*session) != project.getValueOfManagerId() )
        co_return HttpResponse::newNotFoundResponse();

    // User is authorised to delete this project
    co_await Model::Project::deleteByPrimaryKey(projectId);
    co_return HttpResponse::newRedirectionResponse("/", k303SeeOther);
}


}// namespace Ctrlr

