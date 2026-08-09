#version 330 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 FragNormal;

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

float toonStep(float value)
{
    if (value > 0.6) return 1.0;
    if (value > 0.25) return 0.5;
    return 0.0;
}

float toonSpecular(float value)
{
    return value > 0.5 ? 1.0 : 0.0;
}

void main()
{
    if (uUnlit == 1) {
        FragColor = vec4(uObjectColor, uOpacity);
        FragNormal = vec4(0.5, 0.5, 0.5, 1.0);
        return;
    }

    vec4 texSample = texture(uTexture, vTexCoord);
    if (texSample.a < 0.1) {
        discard;
    }

    vec3 texColor = texSample.rgb;
    vec3 objectColor = mix(uObjectColor, texColor, 0.9);  // 90% texture, 10% object color
    
    vec3 normal = normalize(vNormal);
    FragNormal = vec4(normal * 0.5 + 0.5, 1.0);
    vec3 viewDir = normalize(uViewPos - vFragPos);
    vec3 lanternLightDir = normalize(uLight.position - vFragPos);
    vec3 lanternReflectDir = reflect(-lanternLightDir, normal);
    vec3 moonLightDir = normalize(-uMoon.direction);
    vec3 moonReflectDir = reflect(-moonLightDir, normal);

    float ambientStrength = 0.35;
    vec3 ambient = ambientStrength * objectColor;

    float lanternDiff = toonStep(max(dot(normal, lanternLightDir), 0.0));
    float lanternDistance = length(uLight.position - vFragPos);
    float lanternFalloff = 1.0 - toonStep(smoothstep(uLanternRange * 0.4, uLanternRange, lanternDistance)); // @ali keep smooth or not?
    vec3 lanternDiffuse = lanternDiff * objectColor * uLight.color * uLight.intensity * lanternFalloff;

    float specStrength = 0.2;
    float lanternSpec = toonSpecular(pow(max(dot(viewDir, lanternReflectDir), 0.0), 16.0));
    vec3 lanternSpecular = specStrength * lanternSpec * uLight.color * uLight.intensity * lanternFalloff;

    float moonDiff = toonStep(max(dot(normal, moonLightDir), 0.0));
    vec3 moonDiffuse = moonDiff * objectColor * uMoon.color * uMoon.intensity;

    float moonSpec = toonSpecular(pow(max(dot(viewDir, moonReflectDir), 0.0), 8.0));
    vec3 moonSpecular = 0.12 * moonSpec * uMoon.color * uMoon.intensity;

    vec3 color = ambient + moonDiffuse + moonSpecular + lanternDiffuse + lanternSpecular;
    FragColor = vec4(color, uOpacity);
}
