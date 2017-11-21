#include "EnginePCH.h"
#include "Engine.h"

BoxComponent::BoxComponent(ObjectInitData _OI) : WorldComponent(_OI)
{
	m_physicsManager = GetEngine()->GetPhysicsSystem();
	PxMaterial* mat = m_physicsManager->GetPhysXAPI()->createMaterial(1.0f, 1.0f, 0.5f);

	PxBoxGeometry boxGeom = PxBoxGeometry(20, 20, 20);
	m_physxShape = m_physicsManager->GetPhysXAPI()->createShape(boxGeom, *mat);
}

BoxComponent::~BoxComponent()
{

}

PxShape* BoxComponent::GetPhysxShape() const
{
	return m_physxShape;
}