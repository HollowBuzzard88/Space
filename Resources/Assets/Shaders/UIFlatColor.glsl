#type vertex
#version 460 core
layout(location = 0) in vec2 inPos;

uniform mat4 uTransform;
uniform mat4 uViewProj;

void main()
{
    gl_Position = uViewProj * uTransform * vec4(inPos, 0.0, 1.0);
}

#type fragment
#version 460 core

uniform vec4 uCol;

out vec4 fragColor;

void main()
{
    fragColor = uCol;
}
