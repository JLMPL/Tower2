#pragma once
#include "Types.hpp"
#include <string.h>
#include <string>

#define FNV_OFFSET_BASIS 0xcbf29ce484222325
#define FNV_PRIME 0x100000001b3

namespace core
{

template <u32 BufferSize>
class Name
{
public:
    Name() = default;
    Name(const Name&) = default;
    Name(const char* str)
    {
        memcpy(m_data, str, BufferSize);
        updateFNV1Hash();
    }

    Name& operator=(const char* str)
    {
        *this = Name(str);
        return *this;
    }

    bool operator==(const Name& other) const
    {
        return m_hash == other.m_hash;
    }

    bool operator!=(const Name& other) const
    {
        return m_hash != other.m_hash;
    }

    void reset()
    {
        memset(m_data, 0, BufferSize);
    }

    const char* c_str() const
    {
        return m_data;
    }

private:
    void updateFNV1Hash()
    {
        m_hash = FNV_OFFSET_BASIS;

        for (u32 i = 0; i < BufferSize; i++)
        {
            if (m_data[i] == '\0')
                break;

            m_hash *= FNV_PRIME;
            m_hash = m_hash ^ m_data[i];
        }
    }

private:
    char m_data[BufferSize];
    u32 m_size = 0;
    u64 m_hash = 0;
};

using Name16 = Name<16u>;
using Name32 = Name<32u>;
using Name64 = Name<64u>;

/*

Visually

16 XXXXXXXXXXXXXXXX
32 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
64 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

*/

}
