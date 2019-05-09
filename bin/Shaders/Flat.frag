#version 130

in vec3 fragPos;
in vec4 fragColor;
in vec2 fragCoord;
in mat3 fragToTan;
in vec3 shadowSpacePos;

out vec4 outColor;

uniform sampler2D uImage;
uniform sampler2D uNormal;
uniform sampler2D uSpecular;
uniform samplerCube uPointShadowCubemap;

uniform vec3 uCamPos;

struct PointLight
{
    vec3 pos;
    vec3 color;
};

uniform PointLight uPointLights[4];

const float PI = 3.14159265359;

float pointShadow0()
{
    vec3 diff = fragPos - uPointLights[0].pos;
    float dist = length(diff);
    vec3 sample = normalize(diff);

    return (dist < textureCube(uPointShadowCubemap, sample).r + 0.01) ? 1.f : 0.1f;
}

vec4 extractTexture()
{
    return pow(texture(uImage, fragCoord), vec4(2.2));
}

vec3 computeNormal()
{
    vec3 texNormal = texture(uNormal, fragCoord).xyz;
    texNormal.xy = (texNormal.xy * 2) - 1;
    texNormal = normalize(texNormal);

    return normalize(fragToTan * texNormal);
}

vec4 correctGammaAndTonemap(vec3 color)
{
    vec3 mapped = color / (color + vec3(1));
    vec3 final = pow(mapped, vec3(1.0 / 2.2));
    return vec4(final, 1);
}

float distributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / max(denom, 0.001); // prevent divide by zero for roughness=0.0 and NdotH=1.0
}

float geometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = geometrySchlickGGX(NdotV, roughness);
    float ggx1 = geometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 calcPointLight(PointLight pl, vec3 normal, vec3 color, vec3 toCam)
{
    vec3 F0 = vec3(0.04);
    float metallic = 0;
    F0 = mix(F0, color, metallic);

    float roughness = texture(uSpecular, fragCoord).r;

    vec3 tmp = pl.pos - fragPos;
    vec3 toLight = normalize(tmp);
    vec3 halfway = normalize(toLight + toCam);
    vec3 attenuation = pl.color / pow(length(tmp), 2);
    vec3 radiance =  color * attenuation;

    float NDF = distributionGGX(normal, halfway, roughness);
    float G = geometrySmith(normal, toCam, toLight, roughness);
    vec3 F = fresnelSchlick(clamp(dot(halfway, toCam), 0.0, 1.0), F0);

    vec3 nominator = NDF * G * F;
    float denominator = 4 * max(dot(normal, toCam), 0.0) * max(dot(normal, toLight), 0.0);
    vec3 specular = nominator / max(denominator, 0.001);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    float NdotL = max(dot(normal, toLight), 0.0);
    return (kD * color / PI + specular) * radiance * NdotL;
}

void main()
{
    vec3 texture = extractTexture().rgb;
    vec3 normal = computeNormal();
    vec3 toCam = normalize(uCamPos - fragPos);

    vec3 final = texture * vec3(0.01);

    final += calcPointLight(uPointLights[0], normal, texture, toCam) * pointShadow0();
    final += calcPointLight(uPointLights[1], normal, texture, toCam);
    final += calcPointLight(uPointLights[2], normal, texture, toCam);
    final += calcPointLight(uPointLights[3], normal, texture, toCam);

    outColor = correctGammaAndTonemap(final) * fragColor;
}
