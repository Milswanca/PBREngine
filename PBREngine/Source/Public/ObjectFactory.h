#pragma once
#include "WorldObject.h"
#include "World.h"

class ObjectFactory : public ObjectBase
{
public:
	ObjectFactory(ObjectInitData OI);
	~ObjectFactory();

	virtual void Tick(float deltaTime);

	template<typename T>
	T* SpawnWorldObject()
	{
		//Error if T is not a component base
		static_assert(std::is_base_of<WorldObject, T>::value, "SpawnWorldObject must have a template type of WorldObject");

		T* newObj = gEngine->CreateObject<T>(m_world);

		WorldObject* worldObject = static_cast<WorldObject*>(newObj);
		worldObject->Begin();
		m_worldObjects.push_back(worldObject);

		return newObj;
	}

	void DestroyObject(class WorldObject* object)
	{
		auto iter = std::find(m_worldObjects.begin(), m_worldObjects.end(), object);

		if (iter != m_worldObjects.end())
		{
			//m_worldObjects.erase(iter);
			gEngine->DestroyObject(object);
		}
	}

private:
	World* m_world;
	std::vector<class WorldObject*> m_worldObjects;
};

