
// This header exists so QT-Creator doesn't complain about co-routines

#ifndef HPP_COROUTINE
#define HPP_COROUTINE

// __cpp_impl_coroutine is already defined as 201902L, but clang-code model doesn't realise this
// Bypass GCC / MSVC coroutine guards when using clang code model
#if defined(__GNUC__) && defined(__clang__) && !defined(__cpp_impl_coroutine)
    #define __cpp_impl_coroutine 201902L
#elif defined(_MSC_VER) && defined(__clang__) && !defined(__cpp_lib_coroutine)
    #define __cpp_lib_coroutine 201902L
#endif  // if defined(__GNUC__) && defined(__clang__) && !defined(__cpp_impl_coroutine)

// Clang requires coroutine types in std::experimental
#include <coroutine>
#if defined(__clang__)


namespace std::experimental
{


using std::coroutine_traits;
using std::coroutine_handle;
using std::suspend_always;
using std::suspend_never;


}  // namespace std::experimental


#endif  // if defined(__clang__)

#endif // ifndef HPP_COROUTINE
