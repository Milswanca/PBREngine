#version 420 core
#include "Assets/Materials/Default/PBRFunctions.func"

in vec2 UV;
// texture coords

in vec3 world_vertNormal;
in vec3 world_vertPos;
in vec3 world_viewPos;
in vec4 light_fragPos;

struct LightData
{
	vec3 position;
	vec3 colour;
	vec3 direction;
	float intensity;
	float radius;
	int lightType;
};

layout (std140, binding = 1) uniform LightBlock
{
	uniform int NumLights;
	uniform LightData Lights[20];
};

uniform samplerCube MapIrradiance;
uniform sampler2D MapDiffuse;
uniform sampler2D MapNormal;
uniform sampler2D MapMetallic;
uniform sampler2D MapRoughness;
uniform sampler2D MapSpecular;
uniform samplerCube MapPrefilteredEnv;
uniform sampler2D MapBRDF;
uniform sampler2D ShadowMap;

out vec4 FragColor;

uniform float Metallic;
uniform float Roughness;
uniform vec3 Albedo;

float CalculateShadowDirectional(vec4 _fragPos)
{
    // perform perspective divide
    vec3 projCoords = _fragPos.xyz / _fragPos.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(ShadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

	float bias = 0.008;
	float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0; 

    return shadow;
}

void main()
{
    vec2 texCoords = vec2(UV.x, 1 - UV.y);

	vec3 N = world_vertNormal;//normalize(texture( MapNormal, texCoords).rgb * 2.0 - 1.0);
    vec3 V = normalize(world_viewPos - world_vertPos);

	vec3 irradiance = texture(MapIrradiance, N).rgb;
    float ao        = 1; 

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, Albedo, Metallic);
	           
    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < NumLights; ++i) 
    {
        // calculate per-light radiance
  //      vec3 L = normalize(Lights[i].position - world_vertPos);
  //      vec3 H = normalize(V + L);
  //      float distance    = length(Lights[i].position - world_vertPos);
		//float attenuation = clamp(1.0 - distance*distance/(Lights[i].radius*Lights[i].radius), 0.0, 1.0); 
		//attenuation *= attenuation;
  //      vec3 radiance     = Lights[i].colour * attenuation * Lights[i].intensity;        
         
		vec3 L = normalize(Lights[i].direction);
        vec3 H = normalize(V + L);
        vec3 radiance     = Lights[i].colour * Lights[i].intensity;   
		
      	
        // cook-torrance brdf
        float NDF = DistributionGGX(N, H, Roughness);        
        float G   = GeometrySmith(N, V, L, Roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       
      	
        vec3 akS = F;
        vec3 akD = vec3(1.0) - akS;
        akD *= 1.0 - Metallic;	  
      	
        vec3 nominator    = NDF * G * F;
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; 
        vec3 spec     = nominator / denominator;
          
        // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);                
        Lo += (akD * Albedo / PI + spec) * radiance * NdotL;
	}   

	float shadow = (Lights[0].lightType == 2) ? CalculateShadowDirectional(light_fragPos) : 0.0f;

	Lo = min(vec3(1), Lo);
	shadow = max(0, shadow);

	//Diffuse Irradiance
	vec3 kS = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, Roughness);
    vec3 kD = vec3(1.0) - kS;
    vec3 diffuse = irradiance * Albedo;

	//Specular Reflections
	vec3 R = reflect(-V, N);  
	const float MAX_REFLECTION_LOD = 4.0;
	vec3 prefilteredColor = textureLod(MapPrefilteredEnv, R,  Roughness * MAX_REFLECTION_LOD).rgb;   
	vec2 envBRDF  = texture(MapBRDF, vec2(max(dot(N, V), 0.0), Roughness)).rg;
	vec3 specular = prefilteredColor * (kS * envBRDF.x + envBRDF.y);

	//Ambient (final IBL colour)
	vec3 ambient = ((kD * diffuse + specular) * ao) + max(vec3(0), Lo - (shadow));
    
	//Final Colour
	vec3 color = ambient;
	
	//Gamma correction
    //color = color / (color + vec3(1.0));
    //color = pow(color, vec3(1.0/2.2));  
   
    FragColor = vec4(color, 1);
}  