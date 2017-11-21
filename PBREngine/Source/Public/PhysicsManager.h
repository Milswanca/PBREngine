#pragma once
#include "SystemBase.h"

class PhysicsManager : public SystemBase
{
public:
	PhysicsManager(ObjectInitData& OI);
	~PhysicsManager();

	void Init();
	void Update(float _deltaTime);
	void Deinit();

	class PxPhysics* GetPhysXAPI() const;

	class PxRigidStatic* CreatePhysxActorStatic(World* _world, PxShape* _shape);
	class PxRigidDynamic* CreatePhysxActor(World* _world, PxShape* _shape);
	class PxTolerancesScale GetPhysxToleranceScale() const;

private:
	class PxFoundation* m_foundation;
	class PxPhysics* m_physics;

	class PxDefaultErrorCallback m_errorCallback;
	class PxDefaultAllocator m_allocator;

	class PxTolerancesScale m_toleranceScale;
};

