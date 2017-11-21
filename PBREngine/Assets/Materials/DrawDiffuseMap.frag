#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

in vec3 Normal_tangentSpace;
in vec3 FragPos_tangentSpace;
in vec3 ViewPos_tangentSpace;
in vec3 LightPos_tangentSpace;

// Ouput data
out vec4 color;

uniform vec3 AmbientColour;
uniform vec3 DiffuseColour;
uniform vec3 SpecularColour;
uniform float Transparency;
uniform sampler2D MapSpecular;
uniform sampler2D MapNormal;
uniform sampler2D MapDiffuse;

void main()
{
	vec3 lightColor = vec3(1, 1, 1);
	vec3 lightDir = normalize(LightPos_tangentSpace - FragPos_tangentSpace);

    vec3 ambient = AmbientColour * lightColor;

	vec3 norm = normalize(texture(MapNormal, vec2(UV.x, 1.0f - UV.y)).rgb*2.0 - 1.0);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	float specularStrength = 20;
	vec3 viewDir = normalize(ViewPos_tangentSpace - FragPos_tangentSpace);
	vec3 reflectDir = reflect(-lightDir, norm); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor * texture(MapSpecular, vec2(UV.x, 1.0f - UV.y)).xxx; 

	color.xyz = texture(MapDiffuse, vec2(UV.x, 1.0f - UV.y)).xyz;
}