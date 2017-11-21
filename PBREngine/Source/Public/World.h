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
	class PxScene* GetPhysXScene() const;

private:
	class PxScene* m_physxScene = nullptr;
	class PhysicsManager* m_physicsManager = nullptr;

	World* m_thisWorld;
	class ObjectFactory* m_objectFactory;
};