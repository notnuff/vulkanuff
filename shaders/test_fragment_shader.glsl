#version 450

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D texSampler;

struct AmbientLightInfo {
    vec4 color;
};

struct GlobalLightInfo {
    vec3 direction;
    vec4 color;
};

struct PointLightInfo {
    vec3 position;
    float radius;
    vec4 color;
};

layout(binding = 2) uniform Light {
    GlobalLightInfo global;
    AmbientLightInfo ambient;
    PointLightInfo point[4];
} light;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec3 fragPosition;

vec4 GetAmbientColor(AmbientLightInfo ambient, vec4 color) {
    // here I take ambient factor based on how bad fragment is lighten by light source
    float inverseIntencity = 1.0 - color.w;
    float clampedInvIntencity = max(inverseIntencity - 0.5, 0);

    vec4 result = ambient.color;
    result.w *= clampedInvIntencity;

    return result;
}

vec4 GetGlobalLight(GlobalLightInfo global, vec3 normal) {
    float diffuse = max(dot(normal, -global.direction), 0.0);
    vec4 result = global.color;

    result.a *= diffuse;
    return result;
}

vec4 GetPointLight(PointLightInfo light, vec3 normal, vec3 fragPos) {
    vec3 lightDir = light.position - fragPos;
    float distance = length(lightDir);
    if (distance > light.radius) return vec4(0.0);

    lightDir = normalize(lightDir);

    float attenuation = 1.0 - clamp(distance / light.radius, 0.0, 1.0);
    float diffuse = max(dot(normal, lightDir), 0.0);

    vec4 result = {light.color.r, light.color.g, light.color.b, diffuse * attenuation * light.color.w};
    return result;
}

void main() {
    vec3 baseColor = texture(texSampler, fragTexCoord).rgb;
    vec3 normal = normalize(fragNormal);

    vec4 lightColor = vec4(0.0);

    vec4 thisGlobalLight = GetGlobalLight(light.global, normal);
    thisGlobalLight.rgb *= thisGlobalLight.w;
    lightColor += thisGlobalLight;

    vec4 pointLighting = vec4(0.0);
    for (int i = 0; i < 4; ++i) {
        vec4 thisPointLighting = GetPointLight(light.point[i], fragNormal, fragPosition);
        thisPointLighting.rgb *= thisPointLighting.w;
        pointLighting += thisPointLighting;
    }

    lightColor += pointLighting;

    vec4 ambient = GetAmbientColor(light.ambient, lightColor);
    vec3 ambientColor = baseColor * ambient.rgb * ambient.w;

    vec3 baseLightColor = baseColor * lightColor.rgb * lightColor.w + baseColor * ambient.rgb * ambient.w;
    vec3 finalColor = baseLightColor + ambientColor;

    outColor = vec4(finalColor, 1.0);//vec4(clamp(finalColor, 0.0, 1.0), 1.0);
}
