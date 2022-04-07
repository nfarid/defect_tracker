
#ifndef HPP_UTIL_CORE_
#define HPP_UTIL_CORE_

// Only small headers can be included here
#include <cassert>
#include <climits>
#include <cstddef>
#include <cstdint>


/*For some basic core utilities (without making the header file too large), including:
 *
 * byte literal _B;  size_t literal _UZ;  ptrdiff_t literal _Z
 * Finally class for a scope guard
 * UNREACHABLE macro to indicate unreachable path
 * INTERNAL macro to indicate internal linkage
 * IMPLICIT macro to indicate implicit conversion
 * PURE and CONSTPURE macro to indicate pure and const functions
 * DumbPtr to indicate ownership, if smart pointers cannot be used
 * minmax function to get a min max pair, as well as respective min and max functions
 * STRINGIFY and STRINGIFY_MACRO macros to turn tokens into string or evaluated macros into string
 *
 */

namespace Util
{


// To indicate internal linkage
// Because static has too many unrelated meanings
#define UTIL_INTERNAL static


// To explicitly state a constructor or conversion is implicit
// Since there's no [[implicit]] attribute
#define UTIL_IMPLICIT    /*[[implicit]]*/


/**
 *\brief A scope guard to make sure a function is called once and only once at the end of scope
 *
 *\example Finally f = []{cleanup();}; //cleanup is guaranteed to be called
 */
template<typename F>
class Finally final {
public:
    UTIL_IMPLICIT Finally( F&& lambda ) noexcept :
        mFunc{static_cast<F&&>( lambda ) },
        mValid{true}
    {}

    UTIL_IMPLICIT Finally(const F& lambda ) noexcept :
        mFunc{lambda},
        mValid{true}
    {}

    Finally& operator=(Finally&&) = delete;  // No move nor copy

    // mFunc will be called once and only once by the end of Finally's scope
    ~Finally() noexcept {
        (*this)();
    }

    // func can be called early
    void operator()() noexcept {
        if(mValid) {
            mValid = false;
            mFunc();
        }
    }

private:
    F mFunc;
    bool mValid{};
};

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

inline namespace Udl
{


/**
 * @brief Creates a byte literal
 */
UTIL_CONSTPURE std::byte operator"" _B( unsigned long long int x ) {
    assert( x <= UCHAR_MAX );
    return std::byte( x );
}

/**
 * @brief Creates a byte literal
 */
UTIL_CONSTPURE std::byte operator"" _b( unsigned long long int x ) {
    assert( x <= UCHAR_MAX );
    return std::byte( x );
}


}  // namespace Udl


// To indicate owning pointers and when smart pointers can't be used
template<typename T>
using DumbPtr = T*;


/**
 *\brief Obtain a min-max pair from 2 values
 *
 *\example const auto [a, b] = minmax(9, 1);  // a is 1 and b is 9
 */
template<typename T> UTIL_PURE constexpr
auto minmax(const T& lhs, const T& rhs) noexcept {
    using MinMaxPair = struct MinMaxPair {
        T min;
        T max;
    };

    return lhs < rhs ? MinMaxPair{lhs, rhs} : MinMaxPair{rhs, lhs};
}

/**
 *\brief Obtain the minimum from 2 values
 */
template<typename T> UTIL_PURE constexpr
T min(const T& lhs, const T& rhs) noexcept {
    return lhs < rhs ? lhs : rhs;
}

/**
 *\brief Obtain the maximum from 2 values
 */
template<typename T> UTIL_PURE constexpr
T max(const T& lhs, const T& rhs) noexcept {
    return lhs > rhs ? lhs : rhs;
}

// STRINGIFY turns a token into a string (char-array)
// STRINGIFY_MACRO will first evaluate the macro then stringify it
#define UTIL_STRINGIFY_MACRO(S) UTIL_STRINGIFY(S)
#define UTIL_STRINGIFY(S) #S

// An assertion that always assert even on release builds
#ifndef NDEBUG
    #define UTIL_ALWAYS_ASSERT(COND) assert(COND)
#else
#define UTIL_ALWAYS_ASSERT(COND) \
    if(!(COND) ) { \
        Util::Details::abort_with_message_(STRINGIFY(COND), __func__, STRINGIFY_MACRO(__LINE__) ); \
    }
#endif    // ifndef NDEBUG

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



// Details namespace for private internal details to be used by Util only
/*NO-FORMAT*/
namespace Details
{
// Immediately exits (with a failure return) and prints message to stderr
// Only used for ALWAYS_ASSERT macro
[[noreturn]]
void abortWithMessage_(const char* message, const char* func = "", const char* line = "");
}    // namespace Detail
/*YES-FORMAT*/


}// namespace Util


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
