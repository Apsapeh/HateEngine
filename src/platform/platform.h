#pragma once

#if defined(_WIN32)
#define PLATFORM_WINDOWS
#elif defined(__unix__) || defined(__APPLE__)
#define PLATFORM_UNIX
#endif
