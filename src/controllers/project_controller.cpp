
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
    DbClientPtr mDB = app().getDbClient("db");

    CoroMapper<Model::Project> mProjectOrm{mDB};

    HttpResponsePtr newImpl(HttpRequestPtr req, Util::StringMap formData,
            std::string errorMessage);
};

Task<HttpResponsePtr> ProjectController::show(HttpRequestPtr req, int32_t id)
{
    try {
        const Model::Project project = co_await mProjectOrm.findByPrimaryKey(id);
        const Model::Account manager = co_await project.getManager(mDB);
        const std::vector ticketLst = co_await project.getTickets(mDB);

        HttpViewData data = getViewData(project.getValueOfTitle(), *getSession(req) );
        data.insert("project", project.toViewJson() );
        data.insert("manager-name", manager.getValueOfUsername() );
        data.insert("ticket-lst", toViewJson(ticketLst) );
        co_return HttpResponse::newHttpViewResponse("project.csp", data);
    } catch(std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
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
        const Model::Project project = co_await Model::Project::createProject(mProjectOrm, postParams, userId);
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
        const std::vector projectLst = co_await Model::Project::searchProject(mDB, param);
        HttpViewData data = getViewData("project_search", *getSession(req) );
        data.insert("project-lst", toViewJson(projectLst) );
        co_return HttpResponse::newHttpViewResponse("search.csp", data);
    } catch(const std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}

Task<HttpResponsePtr> ProjectController::destroy(HttpRequestPtr req, int32_t id) {
    SessionPtr session = getSession(req);
    if(!isLoggedIn(*session) ) // Not authenticated
        co_return HttpResponse::newNotFoundResponse();
    const Model::Project project = co_await mProjectOrm.findByPrimaryKey(id);
    if(getUserId(*session) != project.getValueOfManagerId() ) // Not authorised (i.e. only project manager can delete their own project)
        co_return HttpResponse::newNotFoundResponse();

    co_await mProjectOrm.deleteByPrimaryKey(id);
    co_return HttpResponse::newRedirectionResponse("/", k303SeeOther);
}


}// namespace Ctrlr

