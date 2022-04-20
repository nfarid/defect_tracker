
#include "auxiliary.hpp"

#include <drogon/HttpFilter.h>


namespace Fltr
{


using namespace drogon;

// This check if the user is logged in
class OnlyLoggedIn : public HttpFilter<OnlyLoggedIn> {
public:
    void doFilter(const HttpRequestPtr& req,
            FilterCallback&& errorCallback,
            FilterChainCallback&& continueCallback) override
    {
        const SessionPtr session = Aux::getSession(req);
        // If the user is logged in, then continue with the request, otherwise return a 401 error
        if(Aux::isLoggedIn(*session) )
            continueCallback();
        else
            errorCallback(HttpResponse::newRedirectionResponse("/login") );
    }

    // This empty constructor is needed
    // because drogon doesn't recognise fitlers without an empty constructor
    // also =default does not work here
    explicit OnlyLoggedIn() {}
};

//

// This check if the token is valid
class ValidToken : public HttpFilter<ValidToken> {
public:
    void doFilter(const HttpRequestPtr& req,
            FilterCallback&& errorCallback,
            FilterChainCallback&& continueCallback) override
    {
        /*NO-FORMAT*/
        #ifndef TEST_
            if(req->parameters().at("token") != Aux::getSession(req)->get<std::string>("token") )
                continueCallback();
            else
                errorCallback(HttpResponse::newNotFoundResponse() );
        #else
            continueCallback();
        #endif  // ifndef TEST_
        /*YES-FORMAT*/
    }

    // This empty constructor is also needed due it being a drogon filter
    explicit ValidToken() {}
};


}// namespace Ctrlr

