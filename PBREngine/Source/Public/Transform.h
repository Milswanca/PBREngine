#pragma once
class Transform
{
public:
	Transform();
	Transform(glm::mat4 inMatrix);
	~Transform();

	void Rotate(float angle, glm::vec3 axis);

	void SetLocalLocation(glm::vec3 location);
	void SetLocalRotation(glm::quat rotation);
	void SetLocalScale(glm::vec3 scale);

	void SetWorldLocation(glm::vec3 location);

	glm::vec3 GetLocalLocation();
	glm::quat GetLocalRotation();
	glm::vec3 GetLocalScale();
	glm::mat4 GetLocalMatrix() const;
	glm::vec3 GetWorldLocation();
	glm::quat GetWorldRotation();
	glm::vec3 GetWorldScale();
	glm::mat4 GetWorldMatrix() const;

	glm::vec3 GetRightVector();
	glm::vec3 GetUpVector();
	glm::vec3 GetForwardVector();

	void SetParent(Transform* _parent);
	void RegisterChild(Transform* _child);
	void DeregisterChild(Transform* _child);

	void UpdateTransform();

private:
	glm::mat4 m_scaleMatrix;
	glm::mat4 m_locationMatrix;
	glm::mat4 m_rotationMatrix;
	glm::mat4 m_localMatrix;
	glm::mat4 m_worldMatrix;

	Transform* m_parent = nullptr;
	std::vector<Transform*> m_children;
};

