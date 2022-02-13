
// All the basic utility headers
#include "./core.hpp"
#include "./cstring_view.hpp"
#include "./misc.hpp"
#include "./size.hpp"
#include "./span.hpp"

#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <string>


// Contains utilities that shouldn't be in header files

namespace Util
{


using std::string_literals::operator""s;


CStringView getEnvironment(CStringView envVar) {
    const char* retval = std::getenv(envVar.c_str() );
    if(!retval)
        throw std::runtime_error("Error: "s + to_string(envVar) + " not in environment");
    return CStringView(retval);
}


namespace Details
{


void abortWithMessage_(const char* message, const char* func, const char* line) {
    std::cerr<<"Abort program: "<<std::endl;
    std::cerr<<"At function: "<<func<<std::endl;
    std::cerr<<"At line: "<<line<<std::endl;
    std::cerr<<message<<std::endl;
    std::cerr<<std::flush;
    std::abort();
}


}  // namespace Details


}  // namespace Util

