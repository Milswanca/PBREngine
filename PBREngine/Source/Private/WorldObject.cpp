#include "EnginePCH.h"
#include "Engine.h"


WorldObject::WorldObject(ObjectInitData OI) : ObjectBase(OI)
{
	m_world = OI.World;
	m_rootComponent = AddComponent<WorldComponent>();
}


WorldObject::~WorldObject()
{

}

void WorldObject::Begin()
{

}

void WorldObject::Tick(float deltaTime)
{
	for (ComponentBase* comp : m_components)
	{
		comp->Tick(deltaTime);
	}
}

void WorldObject::Destroyed()
{
	ObjectBase::Destroyed();

	gEngine->DestroyObject(GetRootComponent());
}

World* WorldObject::GetWorld() const
{
	return m_world;
}

void WorldObject::SetVisibility(bool visible)
{
	bIsVisible = visible;
}

bool WorldObject::GetVisibility() const
{
	return bIsVisible;
}

void WorldObject::Rotate(float angle, glm::vec3 axis)
{
	GetRootComponent()->Rotate(angle, axis);
}

void WorldObject::SetLocation(glm::vec3 location)
{
	GetRootComponent()->SetLocalLocation(location);
}

void WorldObject::SetRotation(glm::quat rotation)
{
	GetRootComponent()->SetLocalRotation(rotation);
}

void WorldObject::SetScale(glm::vec3 scale)
{
	GetRootComponent()->SetLocalScale(scale);
}

Vector3 WorldObject::GetLocation()
{
	return GetRootComponent()->GetLocalLocation();
}

glm::quat WorldObject::GetRotation()
{
	return GetRootComponent()->GetLocalRotation();
}

Vector3 WorldObject::GetScale()
{
	return GetRootComponent()->GetLocalScale();
}

Transform* WorldObject::GetTransform() const
{
	return GetRootComponent()->GetTransform();
}

Vector3 WorldObject::GetRightVector()
{
	return GetRootComponent()->GetRightVector();
}

Vector3 WorldObject::GetUpVector()
{
	return GetRootComponent()->GetUpVector();
}

Vector3 WorldObject::GetForwardVector()
{
	return GetRootComponent()->GetForwardVector();
}
