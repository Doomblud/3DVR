#version 330 core
out vec4 FragColor;

in vec3 vFragPos;
in vec3 vNormal;

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
};

uniform PointLight uLight;
uniform vec3 uViewPos;
uniform vec3 uObjectColor;

void main()
{
    vec3 normal = normalize(vNormal);
    vec3 lightDir = normalize(uLight.position - vFragPos);
    vec3 viewDir = normalize(uViewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float ambientStrength = 0.22;
    vec3 ambient = ambientStrength * uObjectColor;

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * uObjectColor * uLight.color * uLight.intensity;

    float specStrength = 0.35;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specStrength * spec * uLight.color * uLight.intensity;

    vec3 color = ambient + diffuse + specular;
    FragColor = vec4(color, 1.0);
}
