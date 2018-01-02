#include "EnginePCH.h"
#include "Engine.h"

RigidbodyComponent::RigidbodyComponent(ObjectInitData _OI) : WorldComponent(_OI)
{
	m_physicsManager = GetEngine()->GetPhysicsSystem();

	GetTransform()->OnTransformUpdatedDelegate.Add(this, &RigidbodyComponent::OnTransformUpdated);
}

RigidbodyComponent::~RigidbodyComponent()
{

}

void RigidbodyComponent::Tick(float _deltaTime)
{
	WorldComponent::Tick(_deltaTime);

	Vector3 physXVec = Vector3(m_rigidActor->getGlobalPose().p);
	SetWorldLocation(physXVec);
	PxQuat physXQuat = m_rigidActor->getGlobalPose().q;
	SetLocalRotation(glm::quat(physXQuat.w, physXQuat.x, physXQuat.y, physXQuat.z));
}

void RigidbodyComponent::SetKinematic(bool _kinematic)
{	
	m_isKinematic = _kinematic;

	BoxComponent* box = GetOwner()->GetComponent<BoxComponent>();
	PxShape* shape = nullptr;

	if (box)
	{
		shape = box->GetPhysxShape();
	}

	if (_kinematic)
	{
		m_staticActor = m_physicsManager->CreatePhysxActorStatic(GetWorld(), box->GetPhysxShape());
	}
	else
	{
		m_rigidActor = m_physicsManager->CreatePhysxActor(GetWorld(), box->GetPhysxShape());
	}
}

PxActor* RigidbodyComponent::GetPhysxActor() const
{
	if (m_isKinematic)
	{
		return m_staticActor;
	}
	else
	{
		return m_rigidActor;
	}
}

void RigidbodyComponent::OnTransformUpdated(Transform* _transform)
{
	if (m_isKinematic)
	{

	}
	else
	{

	}
}