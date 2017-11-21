#version 420 core
out vec4 FragColor;

in vec3 localPos;
  
uniform samplerCube Skybox;
uniform samplerCube MapIrradiance;
uniform samplerCube MapPrefilter;
uniform samplerCube MapBRDF;

void main()
{
	vec3 envColor = texture(Skybox, localPos).rgb;  
	//vec3 irrColor = texture(MapIrradiance, localPos).rgb;
	//vec3 refColor = texture(MapPrefilter, localPos).rgb;
	//vec3 brdfColor = texture(MapBRDF, localPos).rgb;
	FragColor = vec4(envColor, 1.0);
	//FragColor = vec4(irrColor, 1.0);
	//FragColor = vec4(refColor, 1.0);
	//FragColor = vec4(brdfColor, 1.0);
}