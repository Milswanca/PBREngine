#version 420 core
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec3 vertexTangent;
layout(location = 4) in vec3 vertexBitangent;

layout (std140, binding = 0) uniform CameraBlock
{
	uniform mat4 ViewMatrix;
	uniform mat4 ProjectionMatrix;
	uniform vec3 ViewPos;
};

uniform mat4 ObjectMatrix;
uniform mat4 LightProjection;

out vec3 world_vertNormal;
out vec3 world_vertPos;
out vec3 world_viewPos;
out vec3 world_lightPos;
out vec4 light_fragPos;

out vec2 UV;

void main()
{
	world_vertNormal = normalize(mat3(ObjectMatrix) * vertexNormal);
	world_vertPos = mat3(ObjectMatrix) * vertexPosition;
	world_viewPos = ViewPos;

	light_fragPos = LightProjection * vec4(world_vertPos, 1);

	//mat3 TBN = transpose(mat3(camera_vertTangent, camera_vertBitangent, camera_vertNormal));
	//
	//tangent_vertNormal = TBN * camera_vertNormal;
	//tangent_vertPos = TBN * camera_vertPos;
	//tangent_viewPos = TBN * camera_viewPos;
	//tangent_lightPos = TBN * camera_lightPos;

	UV = vertexUV;

	gl_Position = ProjectionMatrix * ViewMatrix * ObjectMatrix * vec4(vertexPosition, 1);
}