#version 330 core
out vec4 FragColor;

in vec3 vFragPos;
in vec3 vNormal;
in vec2 vTexCoord;

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
};

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
};

uniform PointLight uLight;
uniform DirectionalLight uMoon;
uniform vec3 uViewPos;
uniform vec3 uObjectColor;
uniform sampler2D uTexture;
uniform float uLanternRange;
uniform int uUnlit;
uniform float uOpacity;

void main()
{
    if (uUnlit == 1) {
        FragColor = vec4(uObjectColor, uOpacity);
        return;
    }

    vec3 texColor = texture(uTexture, vTexCoord).rgb;
    vec3 objectColor = mix(uObjectColor, texColor, 0.9);  // 90% texture, 10% object color
    
    vec3 normal = normalize(vNormal);
    vec3 viewDir = normalize(uViewPos - vFragPos);
    vec3 lanternLightDir = normalize(uLight.position - vFragPos);
    vec3 lanternReflectDir = reflect(-lanternLightDir, normal);
    vec3 moonLightDir = normalize(-uMoon.direction);
    vec3 moonReflectDir = reflect(-moonLightDir, normal);

    float ambientStrength = 0.35;
    vec3 ambient = ambientStrength * objectColor;

    float lanternDiff = max(dot(normal, lanternLightDir), 0.0);
    float lanternDistance = length(uLight.position - vFragPos);
    float lanternFalloff = 1.0 - smoothstep(uLanternRange * 0.4, uLanternRange, lanternDistance);
    vec3 lanternDiffuse = lanternDiff * objectColor * uLight.color * uLight.intensity * lanternFalloff;

    float specStrength = 0.2;
    float lanternSpec = pow(max(dot(viewDir, lanternReflectDir), 0.0), 16.0);
    vec3 lanternSpecular = specStrength * lanternSpec * uLight.color * uLight.intensity * lanternFalloff;

    float moonDiff = max(dot(normal, moonLightDir), 0.0);
    vec3 moonDiffuse = moonDiff * objectColor * uMoon.color * uMoon.intensity;

    float moonSpec = pow(max(dot(viewDir, moonReflectDir), 0.0), 8.0);
    vec3 moonSpecular = 0.12 * moonSpec * uMoon.color * uMoon.intensity;

    vec3 color = ambient + moonDiffuse + moonSpecular + lanternDiffuse + lanternSpecular;
    FragColor = vec4(color, uOpacity);
}
