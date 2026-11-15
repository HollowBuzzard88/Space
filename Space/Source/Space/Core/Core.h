#pragma once

#ifdef SP_PLATFORM_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <windows.h>
#elifdef SP_PLATFORM_MAC
    #error No iOS implementation yet!
#elifdef SP_PLATFORM_LINUX
    #error No Linux implementation yet!
#endif

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

// =======================Debug Break=======================
#ifdef _WIN32
    #define DEBUG_BREAK() __debugbreak()
#else
    #ifdef __linux__
        #define DEBUG_BREAK() __builtin_debugtrap()
    #else
        #ifdef __APPLE__
            #define DEBUG_BREAK() __builtin_trap()
        #endif
    #endif
#endif

// =======================Assertiuons=======================
/* #ifdef SP_ENABLE_ASSERTS
    #define SP_ASSERT(x, ...) { if(!(x)) { SP_ERROR("Assertion Fialed: {0}", __VA_ARGS__); DEBUG_BREAK();}}
    #define SP_CORE_ASSERT(x, ...) { if(!(x)) { SP_CORE_ERROR("Assertion Fialed: {0}", __VA_ARGS__); DEBUG_BREAK();}}
#else
    #define SP_ASSERT(x, ...)
    #define SP_CORE_ASSERT(x, ...)
#endif
 */
// =========================Storage=========================
#define BIT(x) 1 << (x)
#define KB(x) ((unsigned long long)1024 * x)
#define MB(x) ((unsigned long long)1024 * KB(x))
#define GB(x) ((unsigned long long)1024 * MB(x))

// =========================Macros==========================
#define SP_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
