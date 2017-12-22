#include "EnginePCH.h"
#include "Engine.h"

DirectionalLight::DirectionalLight(ObjectInitData OI) : Light(OI)
{
	SetColour(Vector3(1, 1, 1));
	SetDirection(Vector3(1, 1, 1));
}

DirectionalLight::~DirectionalLight()
{

}

void DirectionalLight::Begin()
{
	Light::Begin();
}

void DirectionalLight::Tick(float _deltaTime)
{
	Light::Tick(_deltaTime);
}

void DirectionalLight::SetShadowMaterialProperties()
{
	Light::SetShadowMaterialProperties();
	shadowGenMaterial->SetMat4Value("LightSpaceMatrix", GetLightSpaceProjection());
}

void DirectionalLight::SetDirection(Vector3 _direction)
{
	if (_direction == Vector3(0, 0, 0))
	{
		_direction = Vector3(1, 0, 0);
	}

	m_direction = glm::normalize(_direction);
}


void DirectionalLight::AddDisplayProps()
{
	Vector3 direction = m_direction;
	Light::AddDisplayProps();
	ImGui::SliderFloat3("Direction", &direction[0], -1, 1);
	SetDirection(direction);
}

Vector3 DirectionalLight::GetDirection() const
{
	return m_direction;
}

glm::mat4 DirectionalLight::GetLightSpaceProjection()
{
	float near_plane = 0.1f, far_plane = 100.0f;
	glm::mat4 lightProjection = glm::ortho(-300.0f, 300.0f, -300.0f, 300.0f, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt(GetDirection() * 30.0f,
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(0.0f, 1.0f, 0.0f));

	glm::mat4 LightSpaceMatrix = lightProjection * lightView;

	return LightSpaceMatrix;
}

ELightType DirectionalLight::GetLightType() const
{
	return ELightType::LT_Directional;
}

std::vector<std::string> DirectionalLight::GetShaderFiles() const
{
	std::vector<std::string> shaders = std::vector<std::string>
	{ 
		"Assets/Materials/Default/generateDirectionalShadows.vert",
		"Assets/Materials/Default/generateDirectionalShadows.frag" 
	};

	return shaders;
}

EInitDepthType DirectionalLight::GetShadowMapType() const
{
	return EInitDepthType::IDT_As2D;
}

LightData DirectionalLight::GetLightData()
{
	LightData data = Light::GetLightData();
	data.direction = GetDirection();
	return data;
}
