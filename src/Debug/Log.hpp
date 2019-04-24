#pragma once
#include <stdio.h>
#include <stdarg.h>

namespace Log
{
    void info(const char* str, ...);
    void error(const char* str, ...);
    void warning(const char* str, ...);
    void test(const char* str, ...);
}
