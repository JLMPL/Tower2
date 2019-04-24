#include "Log.hpp"

namespace Log
{

#ifdef __unix__

#define RED     "\x1B[1;31m"
#define GREEN   "\x1B[1;32m"
#define YELLOW  "\x1B[1;33m"
#define BLUE    "\x1B[1;34m"
#define MAGENTA "\x1B[1;35m"
#define PURPLE  "\x1B[0;35m"
#define CYAN    "\x1B[1;36m"
#define WHITE   "\x1B[1;37m"
#define RESET   "\x1B[1;0m"

#else

#define RED     ""
#define GREEN   ""
#define YELLOW  ""
#define BLUE    ""
#define MAGENTA ""
#define PURPLE  ""
#define CYAN    ""
#define WHITE   ""
#define RESET   ""

#endif

void info(const char* str, ...)
{
    printf(BLUE "[INFO] " RESET);

    va_list args;
    va_start(args, str);
    vprintf(str, args);
    va_end(args);
}

void error(const char* str, ...)
{
    printf(RED "[INFO] " RESET);

    va_list args;
    va_start(args, str);
    vprintf(str, args);
    va_end(args);
}

void warning(const char* str, ...)
{
    printf(PURPLE "[INFO] " RESET);

    va_list args;
    va_start(args, str);
    vprintf(str, args);
    va_end(args);
}

void test(const char* str, ...)
{
    printf(GREEN "[INFO] " RESET);

    va_list args;
    va_start(args, str);
    vprintf(str, args);
    va_end(args);
}

}
