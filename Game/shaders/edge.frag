#version 330 core

out vec4 FragColor;

in vec2 vTexCoord;

uniform sampler2D uColor;
uniform sampler2D uNormal;
uniform sampler2D uDepth;

uniform vec2 uTexelSize;

void main()
{
    vec3 color = texture(uColor, vTexCoord).rgb;

    // Sample neighbouring pixels.
    vec2 offsets[4] = vec2[](
        vec2(-1.0,  0.0),
        vec2( 1.0,  0.0),
        vec2( 0.0, -1.0),
        vec2( 0.0,  1.0)
    );

    vec3 centerNormal = texture(uNormal, vTexCoord).rgb;

    float normalEdge = 0.0;
    float depthEdge = 0.0;

    float centerDepth = texture(uDepth, vTexCoord).r;

    for (int i = 0; i < 4; ++i)
    {
        vec2 uv = vTexCoord + offsets[i] * uTexelSize;

        vec3 neighbourNormal = texture(uNormal, uv).rgb;
        float neighbourDepth = texture(uDepth, uv).r;

        // Normal discontinuity.
        normalEdge = max(
            normalEdge,
            length(centerNormal - neighbourNormal)
        );

        // Depth discontinuity.
        depthEdge = max(
            depthEdge,
            abs(centerDepth - neighbourDepth)
        );
    }

    // TODO finetune
    float normalThreshold = 0.15;
    float depthThreshold = 0.01;

    bool isEdge =
        normalEdge > normalThreshold ||
        depthEdge > depthThreshold;

    if (isEdge)
    {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
    else
    {
        FragColor = vec4(color, 1.0);
    }
}