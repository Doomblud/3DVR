#version 330 core
layout (location = 0) in vec3 position;

out vec3 vTexCoord;

uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    vTexCoord = position;
    vec4 clipPos = uProjection * uView * vec4(position, 1.0);
    gl_Position = clipPos.xyww;
}
