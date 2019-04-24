#pragma once
#include <vector>

namespace Utils
{
    template <typename T>
    void removeFromVector(std::vector<T>& vec, T thing)
    {
        for (u32 i = 0; i < vec.size(); i++)
        {
            if (vec[i] == thing)
            {
                vec.erase(vec.begin() + i);
                return;
            }
        }
    }

    template <typename T>
    bool isInVector(const std::vector<T>& vec, T thing)
    {
        for (auto& i : vec)
            if (i == thing)
                return true;
        return false;
    }
};
