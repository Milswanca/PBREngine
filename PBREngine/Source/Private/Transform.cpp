#include "EnginePCH.h"
#include "Engine.h"

Transform::Transform()
{

}

Transform::Transform(glm::mat4 inMatrix)
{

}

Transform::~Transform()
{
}

void Transform::Rotate(float angle, Vector3 axis)
{
	m_rotationMatrix *= glm::rotate(angle, (glm::vec3)axis);
	UpdateTransform();
}

void Transform::SetLocalLocation(Vector3 location)
{
	m_locationMatrix = glm::translate((glm::vec3)location);
	UpdateTransform();
}

void Transform::SetLocalRotation(glm::quat rotation)
{
	m_rotationMatrix = glm::mat4_cast(rotation);
	UpdateTransform();
}

void Transform::SetLocalScale(Vector3 scale)
{
	m_scaleMatrix = glm::scale((glm::vec3)scale);
	UpdateTransform();
}

void Transform::SetWorldLocation(Vector3 location)
{
	Vector3 worldLocation = GetWorldLocation();
	Vector3 deltaLocation = location - worldLocation;

	SetLocalLocation(GetLocalLocation() + deltaLocation);
}

Vector3 Transform::GetLocalLocation()
{
	glm::mat4 transformation;
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;

	glm::decompose(m_localMatrix, scale, rotation, translation, skew, perspective);

	return Vector3(translation);
}

glm::quat Transform::GetLocalRotation()
{
	glm::mat4 transformation;
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;

	glm::decompose(m_localMatrix, scale, rotation, translation, skew, perspective);

	rotation = glm::conjugate(rotation);

	return rotation;
}

Vector3 Transform::GetLocalScale()
{
	glm::mat4 transformation;
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;

	glm::decompose(m_localMatrix, scale, rotation, translation, skew, perspective);

	return Vector3(scale);
}

glm::mat4 Transform::GetLocalMatrix() const
{
	return m_localMatrix;
}

Vector3 Transform::GetWorldLocation()
{
	glm::mat4 transformation;
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;

	glm::decompose(m_worldMatrix, scale, rotation, translation, skew, perspective);

	return Vector3(translation);
}

glm::quat Transform::GetWorldRotation()
{
	glm::mat4 transformation;
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;

	glm::decompose(m_worldMatrix, scale, rotation, translation, skew, perspective);

	return rotation;
}

Vector3 Transform::GetWorldScale()
{
	glm::mat4 transformation;
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;

	glm::decompose(m_worldMatrix, scale, rotation, translation, skew, perspective);

	return Vector3(scale);
}

glm::mat4 Transform::GetWorldMatrix() const
{
	return m_worldMatrix;
}

Vector3 Transform::GetRightVector()
{
	return Vector3(glm::normalize(m_worldMatrix[0]));
}

Vector3 Transform::GetUpVector()
{
	return Vector3(glm::normalize(m_worldMatrix[1]));
}

Vector3 Transform::GetForwardVector()
{
	return Vector3(glm::normalize(m_worldMatrix[2]));
}

void Transform::SetParent(Transform* _parent)
{
	if (m_parent)
	{
		m_parent->DeregisterChild(this);
	}

	m_parent = _parent;

	if (m_parent)
	{
		m_parent->RegisterChild(this);
	}
}

void Transform::RegisterChild(Transform* _child)
{
	m_children.push_back(_child);
}

void Transform::DeregisterChild(Transform* _child)
{
	std::remove(m_children.begin(), m_children.end(), _child);
}

void Transform::UpdateTransform()
{
	m_localMatrix = m_locationMatrix * m_rotationMatrix * m_scaleMatrix;
	m_worldMatrix = m_localMatrix;

	if (m_parent)
	{
		m_worldMatrix = m_parent->GetWorldMatrix() * m_worldMatrix;
	}

	for (Transform* i : m_children)
	{
		i->UpdateTransform();
	}

	OnTransformUpdatedDelegate.Invoke(this);
}