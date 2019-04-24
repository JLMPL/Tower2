#pragma once

namespace Assert
{
    static void runtime(bool cond, const char* message, const char* file, u32 line)
    {
        if (!cond)
        {
            printf("%s:%d %s\n", file, line, message);
            u32* ptr = nullptr;
            *ptr = 0;
        }
    }
};

#define ASSERT_R(cond, message) Assert::runtime(cond, message, __FILE__, __LINE__);
