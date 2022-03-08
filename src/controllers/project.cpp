
#include "./auxiliary.hpp"
#include "../models/account.hpp"
#include "../models/project.hpp"
#include "../models/ticket.hpp"

#include <drogon/HttpController.h>

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
        const Criteria projectCriteria{Model::Project::Cols::_project_name, CompareOperator::EQ, query};
        // TODO: Add proper text search
        const std::vector projectLst = mProjectOrm.findBy(projectCriteria);
        HttpViewData data = getViewData("project_search", *getSession(req) );
        data.insert("project_lst", projectLst);
        return cb(HttpResponse::newHttpViewResponse("search.csp", data) );
    }  catch(std::exception& ex) {
        std::cerr<<ex.what()<<std::endl;
        return cb(HttpResponse::newNotFoundResponse() );
    }
}


}// namespace Ctrlr

