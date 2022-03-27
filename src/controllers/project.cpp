
#include "auxiliary.hpp"
#include "../models/account.hpp"
#include "../models/project.hpp"
#include "../models/ticket.hpp"

#include <drogon/HttpController.h>

#include <algorithm>
#include <string>


namespace Ctrlr
{


using namespace Aux;
using namespace drogon;
using namespace drogon::orm;

class Project : public HttpController<Project> {
public:
    /*NO-FORMAT*/
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(Project::show, "/project/{1}", Get);
        ADD_METHOD_TO(Project::search, "/search", Get);
    METHOD_LIST_END
    /*YES-FORMAT*/

    Task<HttpResponsePtr> show(HttpRequestPtr req, int32_t id);
    Task<HttpResponsePtr> search(HttpRequestPtr req);

private:
    DbClientPtr mDB = app().getDbClient("db");

    CoroMapper<Model::Project> mProjectOrm{mDB};
};

Task<HttpResponsePtr> Project::show(HttpRequestPtr req, int32_t id)
{
    try {
        const Model::Project project = co_await mProjectOrm.findByPrimaryKey(id);
        const Model::Account manager = co_await project.getManager(mDB);
        const std::vector ticketLst = co_await project.getTickets(mDB);

        HttpViewData data = getViewData(project.getValueOfTitle(), *getSession(req) );
        data.insert("manager_name", manager.getValueOfUsername() );
        data.insert("ticket_lst", toJson(ticketLst) );
        data.insert("project_id", std::to_string(id) );
        co_return HttpResponse::newHttpViewResponse("project.csp", data);
    } catch(std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}

Task<HttpResponsePtr> Project::search(HttpRequestPtr req){
    const std::string_view param = req->parameters().at("search-project");
    try{
        const std::vector projectLst = co_await Model::Project::searchProject(mDB, param);
        HttpViewData data = getViewData("project_search", *getSession(req) );
        data.insert("project_lst", toJson(projectLst) );
        co_return HttpResponse::newHttpViewResponse("search.csp", data);
    } catch(const std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}


}// namespace Ctrlr

