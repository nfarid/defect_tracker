
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


CStringView get_environment(CStringView env_var) {
    const char* retval = std::getenv(env_var.c_str() );
    if(!retval)
        throw std::runtime_error("Error: "s + to_string(env_var) + " not in environment");
    return CStringView(retval);
}


namespace Details
{


void abort_with_message_(const char* message, const char* func, const char* line) {
    std::cerr<<"Abort program: "<<std::endl;
    std::cerr<<"At function: "<<func<<std::endl;
    std::cerr<<"At line: "<<line<<std::endl;
    std::cerr<<message<<std::endl;
    std::cerr<<std::flush;
    std::abort();
}


}  // namespace Details


}  // namespace Util

