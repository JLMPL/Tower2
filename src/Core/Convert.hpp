#pragma once
#include "Core/Math3D.hpp"
#include <assimp/scene.h>
#define _DEBUG
#include <PhysX/foundation/PxVec3.h>
#include <PhysX/characterkinematic/PxExtended.h>

namespace core::conv
{
    extern vec3 toGlm(const physx::PxVec3& vec);
    extern vec3 toGlm(const physx::PxExtendedVec3& vec);
    extern quat toGlm(const physx::PxQuat& rot);
    extern mat4 toGlm(const physx::PxTransform& mat);

    extern mat4 toGlm(const aiMatrix4x4& v);
    extern vec3 toGlm(const aiVector3D& v);
    extern quat toGlm(const aiQuaternion& v);
    extern vec4 toGlm(const aiColor4D& c);

    extern physx::PxVec3 toPx(const vec3& vec);
    extern physx::PxQuat toPx(const quat& vec);
    extern physx::PxTransform toPx(const mat4& mat);
}
