#pragma once
#include <Core/Types.hpp>
#define SOL_CHECK_ARGUMENTS 1
#include <sol2/sol.hpp>

namespace sol
{
    void checkInitialization();
};

namespace lua = sol;
