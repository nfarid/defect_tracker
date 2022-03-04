
#include "./auxiliary.hpp"

#include <drogon/HttpController.h>


namespace Ctrlr
{


using namespace drogon;
using namespace drogon::orm;
using namespace Aux;


class Ticket : public HttpController<Ticket> {
public:
    /*NO-FORMAT*/
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(Ticket::show, "/project/{1}/ticket/{2}", Get);
    METHOD_LIST_END
    /*YES-FORMAT*/

    void show(const HttpRequestPtr& req, ResponseCallback&& cb, int32_t projectId, int32_t defectId);
};

void Ticket::show(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb,
        int32_t projectId, int32_t defectId)
{
    auto resp=HttpResponse::newHttpResponse();
    resp->setStatusCode(k200OK);
    resp->setContentTypeCode(CT_TEXT_HTML);
    resp->setBody("TODO: Implement");
    cb(resp);
}


}// namespace Ctrlr

