#include "EnginePCH.h"
#include "Engine.h"

World::World(ObjectInitData OI) : ObjectBase(OI)
{
	m_thisWorld = this;
	m_objectFactory = gEngine->CreateObject<ObjectFactory>(this);


	SpawnWorldObject<DirectionalLight>();

	SpawnWorldObject<Camera>();

	SpawnWorldObject<TriangleDrawer>();
}

World::~World()
{
	//delete m_objectFactory;
}

void World::Tick(float deltaTime)
{
	m_objectFactory->Tick(deltaTime);

	ImGui::Begin("Dragon Properties");
	ImGui::SliderFloat("Metallic", &metallic, 0, 1);
	ImGui::SliderFloat("Roughness", &roughness, 0, 1);
	ImGui::ColorEdit3("Albedo", (float*)&albedo);
	ImGui::End();
}

bool World::DestroyObject(WorldObject* object)
{
	if (!m_objectFactory) { return false; }

	m_objectFactory->DestroyObject(object);

	return true;
}

World* World::GetWorld() const
{
	return m_thisWorld;
}

//PxScene* World::GetPhysXScene() const
//{
//	return m_physxScene;
//}

Skybox* World::GetSkybox() const
{
	return m_skybox;
}