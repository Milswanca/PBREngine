#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

in vec3 Normal;
in vec3 FragPos;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D BaseMap;

uniform vec3 ViewPos;

void main()
{
	vec3 lightPos = vec3(0, 10, 10);
	vec3 lightColor = vec3(1, 1, 1);
	float ambientStrength = 0.3f;
    vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	float specularStrength = 6;
	vec3 viewDir = normalize(ViewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor; 

	color = vec4(1, 1, 1, 1) * (vec4(diffuse, 1) + vec4(ambient, 1) + vec4(specular, 1));
}