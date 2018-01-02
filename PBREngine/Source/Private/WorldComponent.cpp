#include "EnginePCH.h"
#include "Engine.h"

WorldComponent::WorldComponent(ObjectInitData OI) : ComponentBase(OI)
{
	m_parent = nullptr;
	m_owner = dynamic_cast<WorldObject*>(OI.Outer);

	m_transform = new Transform();
}


WorldComponent::~WorldComponent()
{

}

World* WorldComponent::GetWorld() const
{
	return m_owner->GetWorld();
}

void WorldComponent::Destroyed()
{
	m_transform->SetParent(nullptr);

	for (WorldComponent* i : m_children)
	{
		gEngine->DestroyObject(i);
	}

	delete m_transform;
}

void WorldComponent::Tick(float _deltaTime)
{
	ComponentBase::Tick(_deltaTime);

	if (GetOwner()->GetVisibility())
	{
		DebugDraw::Line(GetWorldLocation(), GetWorldLocation() + GetForwardVector() * 30.0f, glm::vec4(1, 0, 0, 1));
		DebugDraw::Line(GetWorldLocation(), GetWorldLocation() + GetUpVector() * 30.0f, glm::vec4(0, 1, 0, 1));
		DebugDraw::Line(GetWorldLocation(), GetWorldLocation() + GetRightVector() * 30.0f, glm::vec4(0, 0, 1, 1));
	}
}

void WorldComponent::SetupAttachment(WorldComponent* parent)
{
	if (!parent) { return; }

	parent->AddChild(this);
	m_parent = parent;
	m_transform->SetParent(m_parent->GetTransform());
}

void WorldComponent::AddChild(WorldComponent* child)
{
	m_children.push_back(child);
}

WorldComponent* WorldComponent::GetParentComponent() const
{
	return m_parent;
}

std::vector<WorldComponent*> WorldComponent::GetChildComponents()
{
	return m_children;
}

WorldObject* WorldComponent::GetOwner() const
{
	return m_owner;
}

void WorldComponent::Rotate(float angle, Vector3 axis)
{
	GetTransform()->Rotate(angle, axis);
}

void WorldComponent::SetLocalLocation(Vector3 location)
{
	GetTransform()->SetLocalLocation(location);
}

void WorldComponent::SetLocalRotation(glm::quat rotation)
{
	GetTransform()->SetLocalRotation(rotation);
}

void WorldComponent::SetLocalScale(Vector3 scale)
{
	GetTransform()->SetLocalScale(scale);
}

void WorldComponent::SetWorldLocation(Vector3 location)
{
	GetTransform()->SetWorldLocation(location);
}

Vector3 WorldComponent::GetLocalLocation()
{
	return GetTransform()->GetLocalLocation();
}

glm::quat WorldComponent::GetLocalRotation()
{
	return GetTransform()->GetLocalRotation();
}

Vector3 WorldComponent::GetLocalScale()
{
	return GetTransform()->GetLocalScale();
}

glm::mat4 WorldComponent::GetLocalMatrix() const
{
	return GetTransform()->GetLocalMatrix();
}

Vector3 WorldComponent::GetWorldLocation()
{
	return GetTransform()->GetWorldLocation();
}

glm::quat WorldComponent::GetWorldRotation()
{
	return GetTransform()->GetWorldRotation();
}

Vector3 WorldComponent::GetWorldScale()
{
	return GetTransform()->GetWorldScale();
}

glm::mat4 WorldComponent::GetWorldMatrix() const
{
	return GetTransform()->GetWorldMatrix();
}

Vector3 WorldComponent::GetRightVector()
{
	return GetTransform()->GetRightVector();
}

Vector3 WorldComponent::GetUpVector()
{
	return GetTransform()->GetUpVector();
}

Vector3 WorldComponent::GetForwardVector()
{
	return GetTransform()->GetForwardVector();
}

Transform* WorldComponent::GetTransform() const
{
	return m_transform;
}