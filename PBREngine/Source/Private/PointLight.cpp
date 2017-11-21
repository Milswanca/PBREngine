#include "EnginePCH.h"
#include "Engine.h"

PointLight::PointLight(ObjectInitData OI) : Light(OI)
{
	SetColour(glm::vec3(1, 1, 1));
}

PointLight::~PointLight()
{

}

void PointLight::Begin()
{
	Light::Begin();

	m_cubeProjections = GetEngine()->CreateObject<CubeCamera>(this);
	m_cubeProjections->GetRootComponent()->SetupAttachment(GetRootComponent());
}

void PointLight::SetShadowMaterialProperties()
{
	Light::SetShadowMaterialProperties();

	shadowGenMaterial->SetFloatValue("FarPlane", 100);
	shadowGenMaterial->SetMat4Value("ObjectMatrix", GetTransform()->GetWorldMatrix());
	shadowGenMaterial->SetVector3Value("LightPos", GetLocation());

	m_cubeProjections->SetLocation(GetLocation());
	m_cubeProjections->UpdateProjections();

	for (unsigned int i = 0; i < 6; ++i)
	{
		//m_cubeProjections->GetCamera((ECubeMapSides)i)->SetPerspective(1.0f, 90.0f, 1.0f, 100.0f);
		std::string name = "ShadowMatrices[" + std::to_string(i) + "]";
		shadowGenMaterial->SetMat4Value(name.c_str(), m_cubeProjections->GetCamera((ECubeMapSides)i)->GetProjectionView());
	}
}

void PointLight::AddDisplayProps()
{
	Light::AddDisplayProps();
	ImGui::SliderFloat("Radius", &m_radius, 0, 100);
}

float PointLight::GetRadius() const
{ 
	return m_radius; 
}

void PointLight::SetRadius(float _radius) 
{ 
	m_radius = _radius; 
}

ELightType PointLight::GetLightType() const
{
	return ELightType::LT_Point;
}

std::vector<std::string> PointLight::GetShaderFiles() const
{
	std::vector<std::string> shaders = std::vector<std::string>
	{
		"Assets/Materials/Default/generatePointShadows.vert",
		"Assets/Materials/Default/generatePointShadows.frag",
		"Assets/Materials/Default/generatePointShadows.geom"
	};

	return shaders;
}

EInitDepthType PointLight::GetShadowMapType() const
{
	return EInitDepthType::IDT_AsCube;
}

LightData PointLight::GetLightData()
{
	LightData data = Light::GetLightData();
	data.radius = GetRadius();
	return data;
}
