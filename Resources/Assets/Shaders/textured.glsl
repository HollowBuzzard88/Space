#type vertex
#version 460 core
layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inUV;

uniform mat4 uProj;

out vec2 vUV;

void main()
{
    vUV = inUV;
    gl_Position = uProj * vec4(inPos, 0.0, 1.0);
}

#type fragment
#version 460 core
in vec2 vUV;

uniform sampler2D uAtlas;
uniform vec4 uColor;

out vec4 fragColor;

void main()
{
    float coverage = texture(uAtlas, vUV).r;
    fragColor = vec4(uColor.rgb, uColor.a * coverage);
}
