
#include "./auxiliary.hpp"
#include "../models/comment.hpp"

#include <drogon/HttpController.h>




namespace Ctrlr
{


using namespace Aux;
using namespace drogon;
using namespace drogon::orm;
using std::string_literals::operator""s;

class Comment : public HttpController<Comment> {
public:
    /*NO-FORMAT*/
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(Comment::newForm, "/ticket/{1}/comment", Get);
        ADD_METHOD_TO(Comment::create, "/ticket/{1}/comment", Post);
    METHOD_LIST_END
    /*YES-FORMAT*/

    static void newForm(const HttpRequestPtr& req, ResponseCallback&& cb, int32_t ticketId);
    void create(const HttpRequestPtr& req, ResponseCallback&& cb, int32_t ticketId);

private:
    Mapper<Model::Comment> mCommentOrm = Mapper<Model::Comment>(app().getDbClient("db") );
};

void Comment::newForm(const HttpRequestPtr& req, ResponseCallback&& cb, int32_t ticketId) {
    auto data = getViewData("Post Comment", *getSession(req) );
    data.insert("ticketId", std::to_string(ticketId) );
    return cb(HttpResponse::newHttpViewResponse("comment_form.csp", data) );
}

void Comment::create(const HttpRequestPtr& req, ResponseCallback&& cb, int32_t  ticketId) {
    const auto& postParams = req->parameters();
    try {
        Model::Comment newComment{};
        newComment.setPost(postParams.at("form_post") );
        newComment.setCreatedDate(trantor::Date::now() );
        newComment.setTicketId(ticketId);
        mCommentOrm.insert(newComment);
        return cb(HttpResponse::newRedirectionResponse("/ticket/"s + std::to_string(ticketId) ) );
    } catch(std::exception& ex) {
        std::cerr<<ex.what()<<std::endl;
        return cb(HttpResponse::newNotFoundResponse() );
    }
}


}// namespace Ctrlr

