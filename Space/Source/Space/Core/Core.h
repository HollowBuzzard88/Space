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

// =========================Storage=========================
#define BIT(x) 1 << (x)
#define KB(x) ((unsigned long long)1024 * x)
#define MB(x) ((unsigned long long)1024 * KB(x))
#define GB(x) ((unsigned long long)1024 * MB(x))
