
#ifndef HPP_UTIL_SIZE_
#define HPP_UTIL_SIZE_

#include "core.hpp"

#include <cstddef>
#include <iterator>
#include <type_traits>


// Some functions to get the sizes of things at compile time
namespace Util
{


/**
 *\brief Gets the signed size in bytes of a type
 *
 *\example const x = ssizeof<int>();
 */
template<typename T>
UTIL_CONSTPURE ptrdiff_t ssizeof() noexcept {
    return static_cast<ptrdiff_t>(sizeof(T) );
}

/**
 *\brief Gets the signed size in bytes of a variable
 *
 *\example:
 * const int var = 10;
 * const x = ssizeof(var);
 */
template<typename T>
UTIL_CONSTPURE ptrdiff_t ssizeof(const T&) noexcept {
    return static_cast<ptrdiff_t>(sizeof(T) );
}

using std::size;

#if __cplusplus < 202002L

/**
 * @brief Gets the number of elements of a container
 */
template<typename Container>
UTIL_PURE constexpr auto ssize(const Container& con) noexcept {
    using SSizeType = std::common_type_t<ptrdiff_t, std::make_signed_t<decltype(size(con) )> >;
    return static_cast<SSizeType>( size( con ) );
}

/**
 * @brief Gets the number of elements of an array
 */
template<typename T, ptrdiff_t N>
UTIL_CONSTPURE auto ssize(const T(& )[N]) noexcept {
    return N;
}

#else
// As C++20 already has ssize
using std::ssize;
#endif    // if __cplusplus < 202002L

/**
 * @brief Gets the total size of all the elements in bytes
 */
template<typename Container>
UTIL_PURE constexpr auto size_bytes(const Container& con) noexcept {
    return size(con) * sizeof(typename Container::value_type);
}

/**
 * @brief Gets the total size of an array in bytes
 */
template<typename T, size_t N>
UTIL_PURE constexpr auto size_bytes(const T(& arr)[N]) noexcept {
    return sizeof(arr);
}

/**
 * @brief Gets the total signed size of all the elements in bytes
 */
template<typename Container>
UTIL_PURE constexpr auto ssize_bytes(const Container& con) noexcept {
    return Util::ssize(con) * ssizeof<typename Container::value_type>();
}

/**
 * @brief Gets the total signed size of an array in bytes
 */
template<typename T, ptrdiff_t N>
UTIL_PURE constexpr auto ssize_bytes(const T(& arr)[N]) noexcept {
    return Util::ssizeof(arr);
}

// For iteration
using std::begin;
using std::end;
using std::cbegin;
using std::cend;
using std::rbegin;
using std::rend;
using std::crbegin;
using std::crend;


}    // namespace Util

#endif    // HPP_UTIL_SIZE_
