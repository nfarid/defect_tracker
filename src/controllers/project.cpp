
#include "./auxiliary.hpp"
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
        const Json::Value ticketLstJson = toJson(ticketLst);

        HttpViewData data = getViewData(project.getValueOfTitle(), *getSession(req) );
        data.insert("manager_name", manager.getValueOfUsername() );
        data.insert("ticket_lst", ticketLstJson);
        data.insert("project_id", std::to_string(id) );
        co_return HttpResponse::newHttpViewResponse("project.csp", data);
    } catch(std::exception& ex) {
        std::cerr<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}

Task<HttpResponsePtr> Project::search(HttpRequestPtr req){
    try {
        // action would be /search?search_project=param
        const std::string& param = req->parameters().at("search-project");

        // TODO: Make this more robust (e.g. trim string)
        // query would be like the get param, but with the spaces replaced with '|', allowing searching by token
        std::string query(size(param), '\0');
        std::transform(begin(param), end(param), begin(query), [](char c){return(c==' ' ? '|' : c);});
        if(!query.empty() && (query.back() == '|') ) // query should not end with |
            query.pop_back();

        const auto projectLst = mDB->execSqlSync(
            "SELECT * FROM project WHERE to_tsvector(title) @@ to_tsquery($1)", query);
        Json::Value projectLstJson{};
        for(const auto& project : projectLst) {
            Json::Value projectJson{};
            projectJson["name"] = project["title"].c_str();
            projectJson["id"] = project["id"].c_str();
            projectLstJson.append(std::move(projectJson) );
        }

        HttpViewData data = getViewData("project_search", *getSession(req) );
        data.insert("project_lst", projectLstJson);
        co_return HttpResponse::newHttpViewResponse("search.csp", data);
    }  catch(std::exception& ex) {
        std::cerr<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}


}// namespace Ctrlr

