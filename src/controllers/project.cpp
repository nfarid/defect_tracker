
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
        ADD_METHOD_TO(Project::search, "/search?q={1}", Get);
    METHOD_LIST_END
    /*YES-FORMAT*/

    void show(const HttpRequestPtr& req, ResponseCallback&& cb, int32_t id);
    void search(const HttpRequestPtr& req, ResponseCallback&& cb, const std::string& query);

private:
    Mapper<Model::Account> mAccountOrm = Mapper<Model::Account>(app().getDbClient("db") );
    Mapper<Model::Project> mProjectOrm = Mapper<Model::Project>(app().getDbClient("db") );
    Mapper<Model::Ticket> mTicketOrm = Mapper<Model::Ticket>(app().getDbClient("db") );
};

void Project::show(const HttpRequestPtr& req, ResponseCallback&& cb, int32_t id)
{
    try {
        const Model::Project project = mProjectOrm.findByPrimaryKey(id);
        const Model::Account manager = mAccountOrm.findByPrimaryKey(project.getValueOfManagerId() );

        const Criteria ticketCriteria{Model::Ticket::Cols::_project, CompareOperator::EQ, project.getValueOfId()};
        const std::vector ticketLst = mTicketOrm.findBy(ticketCriteria);

        HttpViewData data = getViewData(project.getValueOfProjectName(), *getSession(req) );
        data.insert("manager_name", manager.getValueOfUsername() );
        data.insert("ticket_lst", ticketLst);
        return cb(HttpResponse::newHttpViewResponse("project.csp", data) );
    } catch(std::exception& ex) {
        std::cerr<<ex.what()<<std::endl;
        return cb(HttpResponse::newNotFoundResponse() );
    }
}

void Project::search(const HttpRequestPtr& req, ResponseCallback&& cb, const std::string& query){
    try {
        std::string tsQuery(size(query), '\0');
        std::transform(begin(query), end(query), begin(tsQuery), [](char c){return(c==' ' ? '|' : c);});

        // TODO: Remove SQL-injection vulnerability here
        auto db = app().getDbClient("db");
        std::string sqlQuery =
                "SELECT * FROM project WHERE to_tsvector(project_name) @@ to_tsquery('"  + tsQuery + "');";
        const auto projectLst = db->execSqlSync(sqlQuery);
        Json::Value projectLstJson{};
        for(const auto& project : projectLst) {
            Json::Value projectJson{};
            projectJson["name"] = project["project_name"].c_str();
            projectJson["id"] = project["id"].c_str();
            projectLstJson.append(std::move(projectJson) );
        }

        HttpViewData data = getViewData("project_search", *getSession(req) );
        data.insert("projectLst", projectLstJson);
        return cb(HttpResponse::newHttpViewResponse("search.csp", data) );
    }  catch(std::exception& ex) {
        std::cerr<<ex.what()<<std::endl;
        return cb(HttpResponse::newNotFoundResponse() );
    }
}


}// namespace Ctrlr

