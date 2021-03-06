/*------------------------------------
  ///\  Plywood C++ Framework
  \\\/  https://plywood.arc80.com/
------------------------------------*/
#pragma once
#include <ply-platform/PlatformDetect.h>
#include <ply-platform/Config.h> // generated header

// clang-format off

//---------------------------------------------
// Global system headers
//---------------------------------------------
#if PLY_TARGET_WIN32 || PLY_TARGET_MINGW     // Win32 API
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #include <windows.h>
    #undef WIN32_LEAN_AND_MEAN
    #undef NOMINMAX
#endif

//---------------------------------------------
// Integer/float types and limits
//---------------------------------------------
#include <stddef.h>
#include <stdint.h>
#include <float.h>

namespace ply {
using s8 = int8_t;
using s16 = int16_t;
using s32 = int32_t;
using s64 = int64_t;
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
#if PLY_PTR_SIZE == 4
using sptr = int32_t;
using uptr = uint32_t;
using sreg = int32_t;
using ureg = uint32_t;
#else
using sptr = int64_t;
using uptr = uint64_t;
using sreg = int64_t;
using ureg = uint64_t;
#endif

//---------------------------------------------
// Limits
//---------------------------------------------
template <typename T> struct Limits;
#define PLY_MAKE_LIMITS(type, lo, hi) \
template <> struct Limits<type> { \
    static constexpr type Min = lo; \
    static constexpr type Max = hi; \
};
PLY_MAKE_LIMITS(s8, INT8_MIN, INT8_MAX)
PLY_MAKE_LIMITS(s16, INT16_MIN, INT16_MAX)
PLY_MAKE_LIMITS(s32, INT32_MIN, INT32_MAX)
PLY_MAKE_LIMITS(s64, INT64_MIN, INT64_MAX)
PLY_MAKE_LIMITS(u8, 0, UINT8_MAX)
PLY_MAKE_LIMITS(u16, 0, UINT16_MAX)
PLY_MAKE_LIMITS(u32, 0, UINT32_MAX)
PLY_MAKE_LIMITS(u64, 0, UINT64_MAX)
PLY_MAKE_LIMITS(float, -FLT_MAX, FLT_MAX)
PLY_MAKE_LIMITS(double, -DBL_MAX, DBL_MAX)

//---------------------------------------------
// Basic comparisons
//---------------------------------------------
template<typename T> inline T abs(T v) { return (v >= 0) ? v : -v; }
template<typename T> inline T min(T a, T b) { return (a < b) ? a : b; }
template<typename T> inline T max(T a, T b) { return (a < b) ? b : a; }
template<typename T> inline T clamp(T arg, T lo, T hi) { return (arg < lo) ? lo : (arg < hi) ? arg : hi; }

struct Empty {};
} // namespace ply

//---------------------------------------------
// Macros
//---------------------------------------------
#if PLY_COMPILER_MSVC
    #include <ply-platform/impl/Compiler_MSVC.h>
#elif PLY_COMPILER_GCC
    #include <ply-platform/impl/Compiler_GCC.h>
#else
    #error "Unsupported platform!"
#endif

#if defined(DEBUG) || defined(_DEBUG) || defined(__DEBUG__)
#define PLY_DEBUG_BUILD 1
#endif

#define PLY_STRINGIFY2(x) #x
#define PLY_STRINGIFY(x) PLY_STRINGIFY2(x)
#define PLY_CAT2(a, b) a ## b
#define PLY_CAT(a, b) PLY_CAT2(a, b)
#define PLY_UNIQUE_VARIABLE(prefix) PLY_CAT(prefix, __LINE__)
#define PLY_PTR_OFFSET(ptr, ofs) ((void*)(((u8*)(void*)(ptr)) + (ofs)))
#define PLY_MEMBER_OFFSET(type, member) offsetof(type, member)
#define PLY_STATIC_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define PLY_UNUSED(x) ((void) x)
#define PLY_CALL_MEMBER(obj, pmf) ((obj).*(pmf))
#define PLY_CALL_MEMBER(obj, pmf) ((obj).*(pmf))
#define PLY_STATIC_ASSERT(x) static_assert(x, #x)

#if PLY_WITH_ASSERTS
    // Use PLY_FORCE_CRASH() since PLY_DEBUG_BREAK() is silently ignored when stepping through Visual Studio:
    #define PLY_ASSERT(cond) do { \
        if (!(cond)) \
            PLY_FORCE_CRASH(); \
    } while (0)
#else
    #define PLY_ASSERT(cond) do {} while (0)
#endif

#define PLY_SFINAE_EXPR_1(name, expr) \
    template <typename T0> static u32 Check##name(std::decay_t<decltype(expr)>*); \
    template <typename T0> static u8 Check##name(...); \
    template <typename T0> static constexpr bool name = (sizeof(Check##name<T0>(nullptr)) == 4);

#define PLY_SFINAE_EXPR_2(name, expr) \
    template <typename T0, typename T1> static u32 Check##name(std::decay_t<decltype(expr)>*); \
    template <typename T0, typename T1> static u8 Check##name(...); \
    template <typename T0, typename T1> static constexpr bool name = (sizeof(Check##name<T0, T1>(nullptr)) == 4);

namespace ply {
PLY_INLINE constexpr u32 swizzle(u32 v) {
    return ((v >> 24) & 0xff) | ((v >> 8) & 0xff00) | ((v << 8) & 0xff0000) | ((v << 24) & 0xff000000u);
}
PLY_INLINE constexpr u16 swizzle(u16 v) {
    return ((v >> 8) & 0xff) | ((v << 8) & 0xff00);
}
} // namespace ply

#if PLY_IS_BIG_ENDIAN
    #define PLY_CONVERT_BIG_ENDIAN(x) (x)
    #define PLY_CONVERT_LITTLE_ENDIAN(x) ::ply::swizzle(x)
#else
    #define PLY_CONVERT_LITTLE_ENDIAN(x) (x)
    #define PLY_CONVERT_BIG_ENDIAN(x) ::ply::swizzle(x)
#endif

//---------------------------------------------
// PLY_DLL_ENTRY
//---------------------------------------------
#if PLY_DLL_IMPORTING
    #define PLY_DLL_ENTRY PLY_DLL_IMPORT
#elif PLY_DLL_EXPORTING
    #define PLY_DLL_ENTRY PLY_DLL_EXPORT
#else
    #define PLY_DLL_ENTRY
#endif
