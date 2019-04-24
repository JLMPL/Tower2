#version 130

in vec3 vertexPos;
in vec3 vertexNormal;
in vec4 vertexColor;
in vec2 vertexCoord;
in ivec4 boneIndices;
in vec4 weights;
in vec3 vertexTangent;
in vec3 vertexBiTangent;

out vec3 fragPos;
out vec4 fragColor;
out vec2 fragCoord;
out mat3 fragToTan;
out vec3 shadowSpacePos;

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;
uniform mat4 uShadow;

#define NUM_BONES 64
uniform mat4 bones[NUM_BONES];

void main()
{
    mat4 boneTransform = bones[boneIndices[0]] * weights[0];
    boneTransform += bones[boneIndices[1]]     * weights[1];
    boneTransform += bones[boneIndices[2]]     * weights[2];
    boneTransform += bones[boneIndices[3]]     * weights[3];

    mat4 boneModel = uModel * boneTransform;
    vec4 worldPos = boneModel * vec4(vertexPos, 1);

    gl_Position = uProj * uView * worldPos;

    fragPos = worldPos.xyz;
    fragColor = vertexColor;
    fragCoord = vertexCoord;

    shadowSpacePos = vec3(uShadow * worldPos);

    mat3 correct = mat3(boneModel);
    vec3 worldNormal = correct * vertexNormal;
    vec3 worldTangent = correct * vertexTangent;
    vec3 worldBiTangent = correct * vertexBiTangent;

    fragToTan = transpose(mat3(
        worldTangent.x, worldBiTangent.x, worldNormal.x,
        worldTangent.y, worldBiTangent.y, worldNormal.y,
        worldTangent.z, worldBiTangent.z, worldNormal.z
    ));
}
