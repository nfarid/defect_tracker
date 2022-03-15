
#include "./auxiliary.hpp"

#include <drogon/HttpController.h>




namespace Ctrlr
{


using namespace Aux;
using namespace drogon;
using namespace drogon::orm;

class Comment : public HttpController<Comment> {
public:
    /*NO-FORMAT*/
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(Comment::newForm, "/ticket/{1}/comment", Get);
    METHOD_LIST_END
    /*YES-FORMAT*/

    void newForm(const HttpRequestPtr& req, ResponseCallback&& cb, int32_t ticketId);
};

void Comment::newForm(const HttpRequestPtr& req, ResponseCallback&& cb, int32_t ticketId) {
    auto data = getViewData("Post Comment", *getSession(req) );
    data.insert("ticketId", std::to_string(ticketId) );
    return cb(HttpResponse::newHttpViewResponse("comment_form.csp", data) );
}


}// namespace Ctrlr

