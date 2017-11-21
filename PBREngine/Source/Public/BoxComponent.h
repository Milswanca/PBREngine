#pragma once
#include "WorldComponent.h"

class BoxComponent : public WorldComponent
{
public:
	BoxComponent(ObjectInitData _OI);
	~BoxComponent();

	class PxShape* GetPhysxShape() const;

private:
	class PxShape* m_physxShape = nullptr;
	class PhysicsManager* m_physicsManager = nullptr;
};

