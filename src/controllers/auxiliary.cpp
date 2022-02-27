
#include "./auxiliary.hpp"

#include <cassert>


namespace Aux
{


using namespace drogon;

SessionPtr getSession(const HttpRequestPtr& req) {
    assert(req && "HttpRequestPtr cannot be null");
    SessionPtr session = req->session();
    if(!session) {
        std::cerr<<"Session is not enabled"<<std::endl;
        throw std::runtime_error("Session is not enabled");
    }
    return session;
}


}  // namespace Aux

