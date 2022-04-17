
#ifndef HPP_UTIL_STRING
#define HPP_UTIL_STRING

#include "cstring_view.hpp"

#include <string>
#include <string_view>


namespace Util
{


using std::string;
using std::string_view;
using std::string_literals::operator""s;
using std::string_view_literals::operator""sv;

// This string contains the whitespace characters
constexpr CStringView whitespaceCharacters = " \f\n\r\t\v";

// Check if a character is a whitespace character
constexpr inline bool isWhitespace(char c) {
    return (c == ' ') ||
           (c == '\t') ||
           (c == '\n') ||
           (c == '\v') ||
           (c == '\f') ||
           (c == '\r');
}

/**
 * @brief returns a view of the string without any leading or trailing whitespace
 */
[[nodiscard]] inline string_view getTrimmedView(string_view str) {
    const size_t beginPos = str.find_first_not_of(whitespaceCharacters);
    if(beginPos == string::npos)
        return "";
    const size_t endPos = str.find_last_not_of(whitespaceCharacters);  // inclusive end-range
    if(endPos == string::npos)
        return "";
    return str.substr(beginPos, endPos+1);
}

/**
 * @brief returns a string without any leading or trailing whitespace
 */
[[nodiscard]] inline string getTrimmed(string_view str) {
    return string(getTrimmedView(str) );
}

/**
 * @brief mutates the string and remove the leading and trailing whitespace
 */
inline void trimIt(string& str) {
    const size_t len = size(str);
    const size_t beginPos = str.find_first_not_of(whitespaceCharacters);
    const size_t endPos = str.find_last_not_of(whitespaceCharacters);  // inclusive end-range
    str.erase(endPos, len-endPos);
    str.erase(0, beginPos);
}

// Checks if a character is valid ascii
constexpr inline bool isAscii(char c) {
    return c <= 127  &&  c >= 0;
}


}  // namespace Util

#endif  // HPP_UTIL_STRING
