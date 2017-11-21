#pragma once
#include "WorldObject.h"

class Box : public WorldObject
{
public:
	Box(ObjectInitData OI);
	~Box();

	class StaticMeshComponent* GetStaticMesh() const;
	class RigidbodyComponent* GetRigidBody() const;

private:
	class RigidbodyComponent* m_rigidBody = nullptr;
	class BoxComponent* m_boxComponent = nullptr;

	class StaticMeshComponent* m_staticMeshComponent = nullptr;
};

