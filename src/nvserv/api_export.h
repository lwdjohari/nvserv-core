#pragma once

#if defined(_WIN32) || defined(_WIN64)
#if defined(NVSERV_BUILD_LOCAL_SHARED) || \
    defined(NVSERV_BUILD_SHARED)  // Building the shared library
#define NVSERV_API __declspec(dllexport)
#elif defined(NVSERV_USE_SHARED)  // Using the shared library
#define NVSERV_API __declspec(dllimport)
#else  // Static build
#define NVSERV_API
#endif
#elif defined(__linux__) || defined(__APPLE__)
#if defined(NVSERV_BUILD_LOCAL_SHARED) || \
    defined(NVSERV_BUILD_SHARED)  // Building the shared library
#define NVSERV_API __attribute__((visibility("default")))
#else  // Static build
#define NVSERV_API
#endif
#else
#define NVSERV_API
#endif