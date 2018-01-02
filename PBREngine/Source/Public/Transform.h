#pragma once
class Transform
{
public:
	Transform();
	Transform(glm::mat4 inMatrix);
	~Transform();

	void Rotate(float angle, Vector3 axis);

	void SetLocalLocation(Vector3 location);
	void SetLocalRotation(glm::quat rotation);
	void SetLocalScale(Vector3 scale);

	void SetWorldLocation(Vector3 location);

	Vector3 GetLocalLocation();
	glm::quat GetLocalRotation();
	Vector3 GetLocalScale();
	glm::mat4 GetLocalMatrix() const;
	Vector3 GetWorldLocation();
	glm::quat GetWorldRotation();
	Vector3 GetWorldScale();
	glm::mat4 GetWorldMatrix() const;

	Vector3 GetRightVector();
	Vector3 GetUpVector();
	Vector3 GetForwardVector();

	void SetParent(Transform* _parent);
	void RegisterChild(Transform* _child);
	void DeregisterChild(Transform* _child);

	void UpdateTransform();

	Delegate<void, Transform*> OnTransformUpdatedDelegate;

private:
	glm::mat4 m_scaleMatrix;
	glm::mat4 m_locationMatrix;
	glm::mat4 m_rotationMatrix;
	glm::mat4 m_localMatrix;
	glm::mat4 m_worldMatrix;

	Transform* m_parent = nullptr;
	std::vector<Transform*> m_children;
};

