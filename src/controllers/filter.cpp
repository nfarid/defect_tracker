
#include "auxiliary.hpp"

#include <drogon/HttpFilter.h>


namespace Fltr
{


using namespace drogon;

// This filter check if the user is logged in
class OnlyLoggedIn : public HttpFilter<OnlyLoggedIn> {
public:
    void doFilter(const HttpRequestPtr& req,
            FilterCallback&& errorCallback, FilterChainCallback&& continueCallback) override
    {
        const SessionPtr session = Aux::getSession(req);
        // If the user is logged in, then continue with the request, otherwise redirect to the login page
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

// This filter check if the CSRF token is valid during a POST request
class ValidToken : public HttpFilter<ValidToken> {
public:
    void doFilter(const HttpRequestPtr& req,
            FilterCallback&& errorCallback, FilterChainCallback&& continueCallback) override
    {
        /*NO-FORMAT*/
        #ifndef TEST_
            //If the token is correct continue, otherwise return an error paget
        try {
            const SessionPtr session = Aux::getSession(req);
            const std::string& sessionToken = session->get<std::string>("token");
            const Util::StringMap& postParams = req->parameters();
            const std::string& sentToken = postParams.at("token");

            if(sentToken == sessionToken)
                continueCallback();
            else
                errorCallback(HttpResponse::newNotFoundResponse() );
        } catch(const std::exception& ex) {
            std::cerr<<__PRETTY_FUNCTION__<<" ; "<<__LINE__<<"\n"<<typeid(ex).name()<<" ; "<<ex.what()<<std::endl;
            errorCallback(HttpResponse::newNotFoundResponse() );
        }
        #else
            continueCallback();
        #endif  // ifndef TEST_
        /*YES-FORMAT*/
    }

    // This empty constructor is also needed due it being a drogon filter
    explicit ValidToken() {}
};


}// namespace Ctrlr

