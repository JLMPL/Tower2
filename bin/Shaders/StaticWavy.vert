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

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;
uniform mat4 uShadow;

uniform float uTime;

void main()
{
    vec3 wind = normalize(vec3(-1,0,-1)) * sin(uTime + vertexColor.g + (vertexPos.x*vertexPos.z*5) - vertexPos.y) * 0.5f;
    wind *= vertexColor.r;

    vec3 worldPos = vec3(uModel * vec4(vertexPos, 1)) + wind;

    gl_Position = uProj * uView * vec4(worldPos, 1);

    fragPos = worldPos;
    fragCoord = vertexCoord;
    fragColor = vec4(1);

    shadowSpacePos = vec3(uShadow * vec4(worldPos, 1));

    mat3 correct = mat3(uModel);
    vec3 worldNormal = correct * vertexNormal;
    vec3 worldTangent = correct * vertexTangent;
    vec3 worldBiTangent = correct * vertexBiTangent;

    fragToTan = transpose(mat3(
        worldTangent.x, worldBiTangent.x, worldNormal.x,
        worldTangent.y, worldBiTangent.y, worldNormal.y,
        worldTangent.z, worldBiTangent.z, worldNormal.z
    ));
}
