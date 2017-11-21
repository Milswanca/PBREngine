#version 420 core
layout (location = 0) in vec3 aPos;

layout (std140, binding = 0) uniform CameraProperties
{
	uniform mat4 ViewMatrix;
	uniform mat4 ProjectionMatrix;
	uniform vec3 ViewPos;
};

out vec3 UVDir;

void main()
{
    UVDir = aPos;
	gl_Position = ProjectionMatrix * ViewMatrix * vec4(aPos, 1.0);
}