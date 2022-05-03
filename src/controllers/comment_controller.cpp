
#include "auxiliary.hpp"

#include "../models/comment.hpp"

#include "../util/form_error.hpp"

#include <drogon/HttpController.h>




namespace Ctrlr
{


using namespace Aux;
using namespace drogon;
using namespace drogon::orm;
using Model::Comment;
using std::string_literals::operator""s;
using std::string;

class CommentController : public HttpController<CommentController> {
public:
    /*NO-FORMAT*/
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(CommentController::newForm, "/ticket/{1}/comment", Get, "Fltr::OnlyLoggedIn");
        ADD_METHOD_TO(CommentController::createSubmit, "/ticket/{1}/comment", Post, "Fltr::OnlyLoggedIn", "Fltr::ValidToken");
    METHOD_LIST_END
    /*YES-FORMAT*/

    // newForm is called whenever the user requests for a form for a new comment
    Task<HttpResponsePtr> newForm(HttpRequestPtr req, IdType ticketId);

    // createSubmit is called whenever the user submits the form data to create a ticket
    Task<HttpResponsePtr> createSubmit(HttpRequestPtr req, IdType ticketId);

private:
    HttpResponsePtr newImpl(HttpRequestPtr req, IdType ticketId, string errorMessage);
};

Task<HttpResponsePtr> CommentController::newForm(HttpRequestPtr req, IdType ticketId) {
    co_return newImpl(req, ticketId, "");
}

HttpResponsePtr CommentController::newImpl(HttpRequestPtr req, IdType ticketId, string errorMessage) {
    auto viewData = getViewData("Post Comment", *getSession(req) );
    viewData.insert("ticket-id", std::to_string(ticketId) );
    viewData.insert("form-error", errorMessage);

    return HttpResponse::newHttpViewResponse("comment_new.csp", viewData);
}

Task<HttpResponsePtr> CommentController::createSubmit(HttpRequestPtr req, IdType ticketId)
{
    const SessionPtr session = getSession(req);
    const IdType userId = getUserId(*session);

    const auto& formData = req->parameters();
    try {
        co_await Comment::createComment(formData, userId, ticketId);
        co_return HttpResponse::newRedirectionResponse("/ticket/"s + std::to_string(ticketId) );  // Redirects to the ticket page once done
    }  catch(const Util::FormError& ex) {
        co_return newImpl(req, ticketId, ex.what() );  // Retry the form if there's a form error
    } catch(const std::exception& ex) {
        std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<ex.what()<<std::endl;
        co_return HttpResponse::newNotFoundResponse();
    }
}


} // namespace Ctrlr

