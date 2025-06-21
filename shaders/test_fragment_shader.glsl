#version 450

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D texSampler;

layout(binding = 2) uniform Light {
    vec3 lightingDirection;
} light;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;

void main() {
    vec3 texColor = texture(texSampler, fragTexCoord).rgb;

    vec3 normal = normalize(fragNormal);

    float diffuse = max(dot(normal, -light.lightingDirection), 0.0);

    // Ambient + diffuse
    vec3 lighting = texColor * (0.1 + diffuse); // 0.1 = ambient factor

    outColor = vec4(lighting, 1.0);
}
