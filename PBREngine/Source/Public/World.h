#pragma once
#include "WorldObject.h"
#include "ObjectFactory.h"

class World : public ObjectBase
{
public:
	World(ObjectInitData OI);
	~World();

	virtual void Tick(float deltaTime);

	template<typename T>
	T* SpawnWorldObject()
	{
		if (!m_objectFactory) { return nullptr; }

		return m_objectFactory->SpawnWorldObject<T>();
	}

	bool DestroyObject(class WorldObject* object);

	virtual World* GetWorld() const override;
	//class PxScene* GetPhysXScene() const;

	class Skybox* GetSkybox() const;

private:
	class Skybox* m_skybox = nullptr;
	//class PxScene* m_physxScene = nullptr;
	class PhysicsManager* m_physicsManager = nullptr;

	World* m_thisWorld;
	class ObjectFactory* m_objectFactory;

	float metallic = 1.0f;
	float roughness = 0.2f;
	glm::vec3 albedo = glm::vec3(1, 1, 1);

	std::vector<class Box*> m_boxes;
};