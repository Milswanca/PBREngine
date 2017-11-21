#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec3 vertexTangent;
layout(location = 4) in vec3 vertexBitangent;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ProjectionViewMatrix;
uniform mat4 MVPMatrix;
uniform mat4 ObjectMatrix;
uniform vec3 ViewPos;
uniform vec3 LightPos;

out vec3 Normal_tangentSpace;
out vec3 FragPos_tangentSpace;
out vec3 ViewPos_tangentSpace;
out vec3 LightPos_tangentSpace;

// Output data ; will be interpolated for each fragment.
out vec2 UV;

void main()
{
	mat3 ModelView3x3 = mat3(ViewMatrix * ObjectMatrix);

    vec3 vertexNormal_cameraspace = ModelView3x3 * normalize(vertexNormal);
    vec3 vertexTangent_cameraspace = ModelView3x3 * normalize(vertexTangent);
    vec3 vertexBitangent_cameraspace = ModelView3x3 * normalize(vertexBitangent);

	mat3 TBN = transpose(mat3(vertexTangent_cameraspace, vertexBitangent_cameraspace, vertexNormal_cameraspace));

	Normal_tangentSpace = TBN * vertexNormal;
	FragPos_tangentSpace = TBN * vertexPosition;
	ViewPos_tangentSpace = TBN * mat3(ViewMatrix) * ViewPos;
	LightPos_tangentSpace = TBN * mat3(ViewMatrix) * LightPos;
	UV = vertexUV;

	gl_Position = ProjectionViewMatrix * ObjectMatrix * vec4(vertexPosition, 1);
}