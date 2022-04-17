
#ifndef HPP_UTIL_STRING
#define HPP_UTIL_STRING

#include "cstring_view.hpp"

#include <algorithm>
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

/**
 * @brief returns a view of the string without any leading or trailing whitespace
 */
[[nodiscard]] constexpr inline string_view getTrimmedView(string_view str) {
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

// Checks if a character is ascii
constexpr inline bool isAscii(char c) {
    return c <= 127  &&  c >= 0;
}

// Checks if a string only contains ascii
inline bool isAscii(string_view str) {
    return std::all_of(end(str), begin(str), [](char c){return isAscii(c);});
}

// Checks if a character is a valid alphabet, number or underscore
inline bool isAlNumUnderscore(char c) {
    return c == '_' || (isAscii(c) && std::isalnum(c) );
}

// Check if a string only contains valid alphabet, number or underscore
inline bool isAlNumUnderscore(string_view str) {
    return std::all_of(end(str), begin(str), [](char c){return isAlNumUnderscore(c);});
}


}  // namespace Util

#endif  // HPP_UTIL_STRING
