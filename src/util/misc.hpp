
#ifndef HPP_UTIL_MISC_
#define HPP_UTIL_MISC_

#include "core.hpp"
#include "cstring_view.hpp"

#include <json/value.h>

#include <charconv>
#include <stdexcept>
#include <string_view>


namespace Util
{


// Contains various miscellaneous utilities that do not belong in other headers


/**
 *\brief Checks if the container contains element with specified element
 */
template<typename C, typename T>
bool contains(const C& container, const T& val) {
    return std::find(cbegin(container), cend(container), val) != cend(container);
}

/**
 * @brief convert a list of string into a JSON
 */
inline Json::Value toJson(const std::vector<std::string_view>& lst) {
    Json::Value json{};
    for(const auto& elem : lst)
        json.append(std::string(elem) );
    return json;
}

/**
 * @brief Obtain the value of the specified environmental variable
 * @throws runtime_error if the specified environmental variable does not exist
 */
inline CStringView getEnvironment(CStringView envVar) {
    const char* retval = std::getenv(envVar.c_str() );
    if(!retval)
        throw std::runtime_error("Error: " + to_string(envVar) + " not in environment");
    return CStringView(retval);
}

/**
 * @brief Converts a string to a number
 * @throws invalid_argument if str does not correspond to a valid number
 * @throws out_of_range if str correspond to a number outside of T's range
 *
 * @example const auto x = str_to_num<int>("210");
 */
template<typename T>
T strToNum(std::string_view str) {
    T out;
    const auto [ptr_, ec] = std::from_chars(str.data(), str.data() + size(str), out);
    if(ec == std::errc{}) // For some reason there isn't a std::errc::success
        return out;
    switch(ec) {
    case std::errc::invalid_argument:
        throw std::invalid_argument{std::string(str) + "is not a valid integer"};
    case std::errc::result_out_of_range:
        throw std::out_of_range{std::string(str) + "is outside int range"};
    default:
        UTIL_UNREACHABLE();
    }
}

/**
 * @brief A function-like class for a more convenient conversion from string to number
 * @throws invalid_argument if the string does not correspond to a valid number
 * @throws out_of_range if the string correspond to a number outside of T's range
 *
 * @example int x = StrToNum{"210"};
 */
using StrToNum = struct [[nodiscard]] StrToNum {
    template<typename T>
    constexpr operator T() {
        return strToNum<T>(m_str_);
    }

    std::string_view m_str_ = "";
};


}  // namespace Util

#endif  // ifndef HPP_UTIL_MISC_
