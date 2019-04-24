#pragma once
#include <stdint.h>
#include <chrono>
#include <string>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define M_TAU (M_PI*2)
#define HALF_PI (M_PI*0.5)

#define EPSILON (0.00001)

using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint16_t;
using u8 = uint8_t;

using i64 = int64_t;
using i32 = int32_t;
using i16 = int16_t;
using i8 = int8_t;

using byte = u8;

using f32 = float;
using f64 = double;

using Bitmask = u32;
using Moment = std::chrono::high_resolution_clock::time_point;
using Seconds = f32;

using Path = std::string;
using Code = std::string;
using String = std::string;
using CString = const char*;

using Identifier = u32;

constexpr f32 operator"" _rad(long double deg)
{
    return deg * (M_PI / 180.f);
}

constexpr f32 operator"" _sec(long double sec)
{
    return f32(sec);
}

constexpr f32 operator"" _sec(unsigned long long int sec)
{
    return f32(sec);
}

constexpr f32 operator"" _ms(long double miles)
{
    return f32(miles / 1000.0);
}

constexpr f32 operator"" _ms(unsigned long long int miles)
{
    return f32(miles) / 1000.0;
}

constexpr u32 operator"" _bytes(unsigned long long int bytes)
{
    return bytes;
}

constexpr u32 operator"" _KB(unsigned long long int bytes)
{
    return bytes * 1024u;
}

constexpr u32 operator"" _MB(unsigned long long int bytes)
{
    return bytes * 1024u * 1024u;
}
