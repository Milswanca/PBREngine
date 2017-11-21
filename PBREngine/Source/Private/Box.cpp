#include "EnginePCH.h"
#include "Engine.h"


Box::Box(ObjectInitData _OI) : WorldObject(_OI)
{
	m_boxComponent = AddComponent<BoxComponent>();
	m_boxComponent->SetupAttachment(GetRootComponent());

	m_rigidBody = AddComponent<RigidbodyComponent>();
	m_rigidBody->SetupAttachment(GetRootComponent());
	m_rigidBody->SetKinematic(false);

	m_staticMeshComponent = AddComponent<StaticMeshComponent>();
	m_staticMeshComponent->SetupAttachment(m_rigidBody);
	m_staticMeshComponent->SetStaticMesh(AssetManager::LoadAsset<StaticMesh>("Assets/StaticMeshes/Render_ball.obj"));
}

Box::~Box()
{
}

StaticMeshComponent* Box::GetStaticMesh() const
{
	return m_staticMeshComponent;
}

RigidbodyComponent* Box::GetRigidBody() const
{
	return m_rigidBody;
}