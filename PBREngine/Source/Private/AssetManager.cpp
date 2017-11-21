#include "EnginePCH.h"
#include "Engine.h"

std::map<std::string, ObjectBase*> AssetManager::m_loadedAssets;
EngineBase* AssetManager::m_engineStatic;

AssetManager::AssetManager(ObjectInitData OI) : ObjectBase(OI)
{

}

void AssetManager::Init(EngineBase* engine)
{
	m_engineStatic = engine;
}

void AssetManager::Deinit()
{
	m_loadedAssets.empty();
}

Material* AssetManager::CreateDefaultMaterial(EMaterialType materialType)
{
	Material* newMat = m_engineStatic->CreateObject<Material>(nullptr);

	switch (materialType)
	{
	case EMaterialType::MT_PBR:
		newMat->AttachShader(LoadAsset<Shader>("Assets/Materials/Default/defaultPBR.vert"));
		newMat->AttachShader(LoadAsset<Shader>("Assets/Materials/Default/defaultPBR.frag"));
		break;
	case EMaterialType::MT_Skybox:
		newMat->AttachShader(LoadAsset<Shader>("Assets/Materials/Default/skybox.vert"));
		newMat->AttachShader(LoadAsset<Shader>("Assets/Materials/Default/skybox.frag"));
		break;
	}

	newMat->Compile();

	return newMat;
}

Material* AssetManager::CreateCustomMaterial()
{
	Material* newMat = m_engineStatic->CreateObject<Material>(nullptr);

	return newMat;
}