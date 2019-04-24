#version 130

in vec3 fragPos;
in vec4 fragColor;
in vec2 fragCoord;
in mat3 fragToTan;
in vec3 shadowSpacePos;

out vec4 outColor;

void main()
{
    outColor = vec4(1);
}
