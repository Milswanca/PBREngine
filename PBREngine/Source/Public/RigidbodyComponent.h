#pragma once
#include "ComponentBase.h"

class RigidbodyComponent : public WorldComponent
{
public:
	RigidbodyComponent(ObjectInitData _OI);
	~RigidbodyComponent();

	virtual void Tick(float _deltaTime) override;

	void SetKinematic(bool _kinematic);

	PxActor* GetPhysxActor() const;

private:
	class PxRigidDynamic* m_rigidActor = nullptr;
	class PxRigidStatic* m_staticActor = nullptr;
	class PxShape* m_shape = nullptr;
	class PhysicsManager* m_physicsManager = nullptr;

	bool m_isKinematic = false;

	void OnTransformUpdated(class Transform* _transform);
};

