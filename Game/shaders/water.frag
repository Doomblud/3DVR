#version 330 core
out vec4 FragColor;

in vec3 vFragPos;
in vec3 vNormal;

uniform samplerCube uSkybox;
uniform vec3 uViewPos;
uniform float uTime;

void main()
{
    vec3 normal = normalize(vNormal);
    normal.xz += vec2(
        sin(vFragPos.z * 1.7 + uTime * 0.9),
        cos(vFragPos.x * 1.4 + uTime * 0.7)) * 0.055;
    normal = normalize(normal);

    vec3 incident = normalize(vFragPos - uViewPos);
    vec3 reflectedDir = reflect(incident, normal);
    vec3 refractedDir = refract(incident, normal, 1.0 / 1.33);

    vec3 reflected = texture(uSkybox, reflectedDir).rgb;
    vec3 refracted = texture(uSkybox, refractedDir).rgb;
    vec3 waterTint = vec3(0.05, 0.20, 0.24);

    float fresnel = pow(1.0 - max(dot(-incident, normal), 0.0), 3.0);
    vec3 color = mix(refracted * waterTint, reflected, 0.35 + fresnel * 0.45);
    color += vec3(0.02, 0.08, 0.09);

    FragColor = vec4(color, 0.72);
}
