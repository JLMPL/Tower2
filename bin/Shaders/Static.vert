#version 130

in vec3 vertexPos;
in vec3 vertexNormal;
in vec4 vertexColor;
in vec2 vertexCoord;
in vec3 vertexTangent;
in vec3 vertexBiTangent;

out vec3 fragPos;
out vec2 fragCoord;
out vec4 fragColor;
out mat3 fragToTan;
out vec3 shadowSpacePos;
out vec3 fragNormal;

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;
uniform mat4 uShadow;

void main()
{
    vec4 worldPos = uModel * vec4(vertexPos, 1);
    gl_Position = uProj * uView * worldPos;

    fragPos = worldPos.xyz;
    fragColor = vertexColor;
    fragCoord = vertexCoord;

    shadowSpacePos = vec3(uShadow * worldPos);

    mat3 correct = mat3(uModel);
    vec3 worldNormal = correct * vertexNormal;
    vec3 worldTangent = correct * vertexTangent;
    vec3 worldBiTangent = correct * vertexBiTangent;

    fragToTan = transpose(mat3(
        worldTangent.x, worldBiTangent.x, worldNormal.x,
        worldTangent.y, worldBiTangent.y, worldNormal.y,
        worldTangent.z, worldBiTangent.z, worldNormal.z
    ));

    fragNormal = worldNormal;
}
