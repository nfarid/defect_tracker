
#include "auxiliary.hpp"
#include "../models/comment.hpp"
#include "../util/form_error.hpp"

#include <drogon/HttpController.h>




namespace Ctrlr
{


using namespace Aux;
using namespace drogon;
using namespace drogon::orm;
using std::string_literals::operator""s;

class CommentController : public HttpController<CommentController> {
public:
    /*NO-FORMAT*/
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(CommentController::newForm, "/ticket/{1}/comment", Get);
        ADD_METHOD_TO(CommentController::create, "/ticket/{1}/comment", Post);
    METHOD_LIST_END
    /*YES-FORMAT*/

    Task<HttpResponsePtr> newForm(HttpRequestPtr req, int32_t ticketId);
    Task<HttpResponsePtr> create(HttpRequestPtr req, int32_t ticketId);

private:
    CoroMapper<Model::Comment> mCommentOrm{app().getDbClient("db")};

    HttpResponsePtr newImpl(HttpRequestPtr req, int32_t ticketId);
};

Task<HttpResponsePtr> CommentController::newForm(HttpRequestPtr req, int32_t ticketId) {
    co_return newImpl(req, ticketId);
}

HttpResponsePtr CommentController::newImpl(HttpRequestPtr req, int32_t ticketId) {
    const SessionPtr session = getSession(req);
    if(!isLoggedIn(*session) ) // Cannot create a ticket if not logged in
        return HttpResponse::newRedirectionResponse("/");

    auto data = getViewData("Post Comment", *getSession(req) );
    data.insert("ticket_id", std::to_string(ticketId) );
    return HttpResponse::newHttpViewResponse("comment_form.csp", data);
}

Task<HttpResponsePtr> CommentController::create(HttpRequestPtr req, int32_t ticketId)
{
    const SessionPtr session = getSession(req);
    if(!isLoggedIn(*session) ) // Cannot create a comment if not logged in
        co_return HttpResponse::newRedirectionResponse("/");
    const int32_t userId = session->get<int32_t>("user_id");

    const auto& postParams = req->parameters();
    try {
        co_await Model::Comment::createComment(mCommentOrm, postParams, userId, ticketId);
    }  catch(const Util::FormError& ex) {
        // TODO: Let the user retry
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<ex.what()<<std::endl;
        co_return newImpl(req, ticketId);
    } catch(const std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}


} // namespace Ctrlr

