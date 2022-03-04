
#include "./auxiliary.hpp"
#include "../models/account.hpp"
#include "../models/project.hpp"
#include "../models/ticket.hpp"

#include <drogon/HttpController.h>


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
    METHOD_LIST_END
    /*YES-FORMAT*/

    void show(const HttpRequestPtr& req, ResponseCallback&& cb, int32_t id);

private:
    Mapper<Model::Account> mAccountOrm = Mapper<Model::Account>(app().getDbClient("db") );
    Mapper<Model::Project> mProjectOrm = Mapper<Model::Project>(app().getDbClient("db") );
    Mapper<Model::Ticket> mTicketOrm = Mapper<Model::Ticket>(app().getDbClient("db") );
};

void Project::show(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb, int32_t id)
{
    try {
        const auto project = mProjectOrm.findByPrimaryKey(id);
        const auto manager = mAccountOrm.findByPrimaryKey(project.getValueOfManagerId() );
        HttpViewData data = getViewData(project.getValueOfProjectName(), *getSession(req) );
        data.insert("manager_name", manager.getValueOfUsername() );
        return cb(HttpResponse::newHttpViewResponse("project.csp", data) );
    } catch(std::exception& ex) {
        std::cerr<<ex.what()<<std::endl;
        return cb(HttpResponse::newNotFoundResponse() );
    }
}


}// namespace Ctrlr

