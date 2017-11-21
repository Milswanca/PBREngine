#include "EnginePCH.h"
#include "Engine.h"

#include "PhysX\extensions\PxExtensionsAPI.h"

PhysicsManager::PhysicsManager(ObjectInitData& _OI) : SystemBase(_OI)
{

}

PhysicsManager::~PhysicsManager()
{

}

void PhysicsManager::Init()
{
	m_foundation = PxCreateFoundation(PX_FOUNDATION_VERSION, m_allocator, m_errorCallback);

	if (!m_foundation)
	{
		sprintf("PxCreateFoundation failed!", "%s\n");
	}

	m_toleranceScale = PxTolerancesScale();

	m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, m_toleranceScale, true, nullptr);
	if (!m_physics)
	{
		sprintf("PxCreatePhysics failed!", "%s\n");
	}
}

void PhysicsManager::Update(float _deltaTime)
{

}

void PhysicsManager::Deinit()
{
	m_physics->release();
}

PxRigidStatic* PhysicsManager::CreatePhysxActorStatic(World* _world, PxShape* _shape)
{
	PxTransform pose = PxTransform(PxVec3(0, 0, 0));
	PxRigidStatic* actor = GetPhysXAPI()->createRigidStatic(pose);

	if (_shape)
	{
		actor->attachShape(*_shape);
	}

	_world->GetPhysXScene()->addActor(*actor);

	return actor;
}

PxRigidDynamic* PhysicsManager::CreatePhysxActor(World* _world, PxShape* _shape)
{
	PxTransform pose = PxTransform(PxVec3(0, 0, 0));
	PxRigidDynamic* actor = GetPhysXAPI()->createRigidDynamic(pose);

	if (_shape)
	{
		actor->attachShape(*_shape);
	}

	_world->GetPhysXScene()->addActor(*actor);

	return actor;
}

PxPhysics* PhysicsManager::GetPhysXAPI() const
{
	return m_physics;
}

PxTolerancesScale PhysicsManager::GetPhysxToleranceScale() const
{
	return m_toleranceScale;
}