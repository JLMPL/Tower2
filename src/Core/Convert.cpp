#include "Convert.hpp"

namespace core::conv
{

vec3 toGlm(const physx::PxVec3& vec)
{
    return vec3(vec.x, vec.y, vec.z);
}

vec3 toGlm(const physx::PxExtendedVec3& vec)
{
    return vec3(vec.x, vec.y, vec.z);
}

quat toGlm(const physx::PxQuat& rot)
{
    return quat(rot.w, rot.x, rot.y, rot.z);
}

mat4 toGlm(const physx::PxTransform& mat)
{
    vec3 pos = toGlm(mat.p);
    quat rot = toGlm(mat.q);

    return math::translate(pos) * math::mat4_cast(rot);
}

mat4 toGlm(const aiMatrix4x4& v)
{
    mat4 out;
    memcpy(&out, &v, sizeof(v));
    return glm::transpose(out);
}

vec3 toGlm(const aiVector3D& v)
{
    vec3 out;
    memcpy(&out, &v, sizeof(v));
    return out;
}

quat toGlm(const aiQuaternion& v)
{
    return quat(v.w, v.x, v.y, v.z);
}

vec4 toGlm(const aiColor4D& c)
{
    return vec4(c.r, c.g, c.b, c.a);
}

physx::PxVec3 toPx(const vec3& vec)
{
    return physx::PxVec3(vec.x, vec.y, vec.z);
}

physx::PxQuat toPx(const quat& qt)
{
    return physx::PxQuat(qt.x, qt.y, qt.z, qt.w);
}

physx::PxTransform toPx(const mat4& mat)
{
    physx::PxVec3 pos = toPx(vec3(mat[3]));
    physx::PxQuat rot = toPx(math::quat_cast(mat));

    return physx::PxTransform(pos, rot);
}

}
