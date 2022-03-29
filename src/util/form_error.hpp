
#ifndef HPP_UTIL_FORM_ERROR
#define HPP_UTIL_FORM_ERROR

#include <stdexcept>
#include <string_view>


namespace Util
{


// A simple class to indicate a error from a form
class FormError : public std::runtime_error {
public:
    explicit FormError(const std::string_view str) :
        std::runtime_error{std::string(str)}
    {}
};


}// namespace Util

#endif  // ifndef HPP_UTIL_FORM_ERROR
