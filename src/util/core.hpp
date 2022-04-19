
#ifndef HPP_UTIL_CORE_
#define HPP_UTIL_CORE_

// Only small headers can be included here
#include <cassert>
#include <climits>
#include <cstddef>
#include <cstdint>


// For some basic core utilities

namespace Util
{


// To explicitly state a constructor or conversion is implicit
// Since there's no [[implicit]] attribute
#define UTIL_IMPLICIT    /*[[implicit]]*/


// To indicate pure functions (i.e. it only returns a value and has no side effects)
#if defined( __GNUG__ )
    #define UTIL_PURE [[nodiscard]]  __attribute__( (pure) )
#else
    #define PURE [[nodiscard]]
#endif  // if defined( __GNUG__ )

// const is like pure but cannot take pointer/reference arguments nor read static data
// constpure indicates a function that is constexpr and const
#if defined( __GNUG__ )
    #define UTIL_CONSTPURE [[nodiscard]]  __attribute__( (const) ) constexpr
#else
    #define CONSTPURE [[nodiscard]] constexpr inline
#endif  // if defined( __GNUG__ )


/*
 * To indicate that a code path should never be reached
 *
 * Used to silence false positive warnings in a fully handled enum switch
 *
 * although default should not be used, rather it should be after the switch statement
 * This is to prevent useful warnings (e.g. -Wswitch) from being silenced
 *
 * -Wcovered-switch-default is a good warning to have (only clang has it)
 * -Wswitch-enum is very bad (just use -Wswitch)
 *
 */
/*NO-FORMAT*/
#if defined( __GNUG__ )
    #define UTIL_UNREACHABLE() do{ assert(false && "unreachable"); __builtin_unreachable(); }while(false)
#elif defined( _MSC_VER )
    #define UTIL_UNREACHABLE() do{ assert(false && "unreachable"); __assume(0); }while(false)
#else
    #define UTIL_UNREACHABLE() do{ assert(false && "unreachable"); }while(false)
#endif    // if defined( __GNUG__ )
/*YES-FORMAT*/


}  // namespace Util


// These are already in the global namespace anyway, and should be
using std::int8_t, std::int16_t, std::int32_t, std::int64_t;
using std::int_fast8_t, std::int_fast16_t, std::int_fast32_t, std::int_fast64_t;
using std::int_least8_t, std::int_least16_t, std::int_least32_t, std::int_least64_t;
using std::intmax_t, std::intptr_t;
using std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t;
using std::uint_fast8_t, std::uint_fast16_t, std::uint_fast32_t, std::uint_fast64_t;
using std::uint_least8_t, std::uint_least16_t, std::uint_least32_t, std::uint_least64_t;
using std::uintmax_t, std::uintptr_t;

#endif    // HPP_UTIL_CORE_
