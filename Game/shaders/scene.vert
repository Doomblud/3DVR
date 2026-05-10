#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coord;
layout (location = 2) in vec3 normal;

out vec3 vFragPos;
out vec3 vNormal;
out vec2 vTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    vec4 worldPos = uModel * vec4(position, 1.0);
    vFragPos = worldPos.xyz;

    mat3 normalMatrix = mat3(transpose(inverse(uModel)));
    vNormal = normalize(normalMatrix * normal);
    vTexCoord = tex_coord;

    gl_Position = uProjection * uView * worldPos;
}
