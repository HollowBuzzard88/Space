#version 460 core
layout (location = 0) in vec3 vPos;

uniform mat4 uViewProj;
uniform mat4 uTransform;

void main()
{
    gl_Position = uViewProj * uTransform * vec4(vPos, 1.0);
}
