#include "EnginePCH.h"
#include "Engine.h"

Light::Light(ObjectInitData OI) : WorldObject(OI)
{
	m_renderManager = GetEngine()->GetRenderSystem();

	if (m_renderManager)
	{
		m_renderManager->RegisterLight(this);
	}
}

Light::~Light()
{

}

void Light::Begin()
{
	WorldObject::Begin();

	shadowGenMaterial = AssetManager::CreateCustomMaterial();
	std::vector<std::string> shaders = GetShaderFiles();

	for (int i = 0; i < shaders.size(); ++i)
	{
		shadowGenMaterial->AttachShader(AssetManager::LoadAsset<Shader>(shaders[i]));
	}

	shadowGenMaterial->Compile();

	shadowRenderer = RenderTarget::Create(this, GetShadowMapSize(), GetShadowMapType());
	shadowTexture = shadowRenderer->GetDepthTexture();
}

void Light::Tick(float _deltaTime)
{
	WorldObject::Tick(_deltaTime);

	ImGui::Begin("Light Properties");
	AddDisplayProps();
	ImGui::End();

	glEnable(GL_DEPTH_TEST);
	SetShadowMaterialProperties();
	shadowGenMaterial->Use();
	shadowRenderer->SetAsActiveTarget();
	glClear(GL_DEPTH_BUFFER_BIT);

	GetEngine()->GetRenderSystem()->RenderScene(nullptr, ERenderConditions::RC_All);

	shadowRenderer->RemoveAsActiveTarget();
	shadowGenMaterial->Unuse();
}

void Light::SetShadowMaterialProperties()
{

}

void Light::AddDisplayProps()
{
	ImGui::SliderFloat3("Position", &position[0], -100, 100);
	ImGui::SliderFloat("Intensity", &m_intensity, 0, 100);
	ImGui::ColorEdit3("Light Colour", &m_colour[0]);
	SetLocation(position);
}

std::vector<std::string> Light::GetShaderFiles() const
{
	return std::vector<std::string>();
}

float Light::GetIntensity()
{ 
	return m_intensity; 
}

void Light::SetIntensity(float _intensity)
{ 
	m_intensity = _intensity; 
}

glm::vec3 Light::GetColour()
{ 
	return m_colour; 
}

void Light::SetColour(glm::vec3 _colour)
{ 
	m_colour = _colour; 
}

unsigned int Light::GetLightID() const
{ 
	return m_lightID; 
}

void Light::SetLightID(unsigned int _id)
{ 
	m_lightID = _id; 
}

ELightType Light::GetLightType() const
{ 
	return ELightType::LT_None; 
}

Texture* Light::GetShadowMap() const
{
	return shadowTexture;
}

unsigned int Light::GetShadowMapSize() const
{
	return 8192;
}

EInitDepthType Light::GetShadowMapType() const
{
	return EInitDepthType::IDT_As2D;
}

LightData Light::GetLightData()
{
	LightData newData = LightData(GetLocation(), GetColour(), glm::vec3(0), GetIntensity(), 0.0f, 0);
	return newData;
}