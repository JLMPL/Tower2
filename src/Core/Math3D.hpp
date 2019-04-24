#pragma once
#include "Types.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using vec2 = glm::tvec2<f32, (glm::precision)0u>;
using vec2i = glm::tvec2<i32, (glm::precision)0u>;
using vec2u = glm::tvec2<u32, (glm::precision)0u>;

using vec3 = glm::tvec3<f32, (glm::precision)0u>;
using vec4 = glm::tvec4<f32, (glm::precision)0u>;

using mat3 = glm::tmat3x3<f32, (glm::precision)0u>;
using mat4 = glm::tmat4x4<f32, (glm::precision)0u>;

using quat = glm::tquat<f32, (glm::precision)0u>;

using Color = glm::tvec4<f32, (glm::precision)0u>;
using UvCoord = vec2;

namespace glm
{
    const vec3 vecX = {1,0,0};
    const vec3 vecY = {0,1,0};
    const vec3 vecZ = {0,0,1};
    const vec3 vec0 = {0,0,0};

    extern mat4 rotate(f32 angle, const vec3& axis);

    extern void printMatrix(const mat4& mat);
}

namespace math = glm;
