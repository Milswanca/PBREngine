#include "EnginePCH.h"
#include "ObjectFactory.h"
#include "WorldObject.h"

ObjectFactory::ObjectFactory(ObjectInitData OI) : ObjectBase(OI)
{
	m_world = OI.World;
}

ObjectFactory::~ObjectFactory()
{
	//while (m_worldObjects.size() > 0)
	//{
	//	DestroyObject(m_worldObjects[0]);
	//}
}

void ObjectFactory::Tick(float deltaTime)
{
	for (int i = 0; i < m_worldObjects.size(); ++i)
	{
		m_worldObjects[i]->Tick(deltaTime);
	}
}