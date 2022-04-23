
#include "auxiliary.hpp"

#include "../models/comment.hpp"

#include "../util/database.hpp"
#include "../util/form_error.hpp"

#include <drogon/HttpController.h>




namespace Ctrlr
{


using namespace Aux;
using namespace drogon;
using namespace drogon::orm;
using std::string_literals::operator""s;
using std::string;

class CommentController : public HttpController<CommentController> {
public:
    /*NO-FORMAT*/
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(CommentController::newForm, "/ticket/{1}/comment", Get, "Fltr::OnlyLoggedIn");
        ADD_METHOD_TO(CommentController::create, "/ticket/{1}/comment", Post, "Fltr::OnlyLoggedIn", "Fltr::ValidToken");
    METHOD_LIST_END
    /*YES-FORMAT*/

    Task<HttpResponsePtr> newForm(HttpRequestPtr req, IdType ticketId);
    Task<HttpResponsePtr> create(HttpRequestPtr req, IdType ticketId);

private:
    CoroMapper<Model::Comment> mCommentOrm{Util::getDb()};

    HttpResponsePtr newImpl(HttpRequestPtr req, IdType ticketId, string errorMessage);
};

Task<HttpResponsePtr> CommentController::newForm(HttpRequestPtr req, IdType ticketId) {
    co_return newImpl(req, ticketId, "");
}

HttpResponsePtr CommentController::newImpl(HttpRequestPtr req, IdType ticketId, string errorMessage) {
    auto data = getViewData("Post Comment", *getSession(req) );
    data.insert("ticket-id", std::to_string(ticketId) );
    data.insert("form-error", errorMessage);
    return HttpResponse::newHttpViewResponse("comment_new.csp", data);
}

Task<HttpResponsePtr> CommentController::create(HttpRequestPtr req, IdType ticketId)
{
    const SessionPtr session = getSession(req);
    const IdType userId = getUserId(*session);

    const auto& postParams = req->parameters();
    try {
        co_await Model::Comment::createComment(postParams, userId, ticketId);
        co_return HttpResponse::newRedirectionResponse("/");
    }  catch(const Util::FormError& ex) {
        // TODO: Let the user retry
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<ex.what()<<std::endl;
        co_return newImpl(req, ticketId, ex.what() );
    } catch(const std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}


} // namespace Ctrlr

