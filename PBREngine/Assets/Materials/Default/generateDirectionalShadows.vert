#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 LightSpaceMatrix;
uniform mat4 ObjectMatrix;

void main()
{
    gl_Position = LightSpaceMatrix * ObjectMatrix * vec4(aPos, 1.0);
}  