#pragma once
#include "ComponentBase.h"
#include "Transform.h"

class WorldComponent : public ComponentBase
{
public:
	WorldComponent(ObjectInitData OI);
	~WorldComponent();

	virtual void Tick(float _deltaTime) override;
	virtual void Destroyed() override;

	Transform* GetTransform() const;

	void SetupAttachment(WorldComponent* parent);
	void AddChild(WorldComponent* child);

	WorldComponent* GetParentComponent() const;
	std::vector<WorldComponent*> GetChildComponents();

	WorldObject* GetOwner() const;

	virtual class World* GetWorld() const override;

private:
	WorldObject* m_owner = nullptr;
	WorldComponent* m_parent = nullptr;
	std::vector<WorldComponent*> m_children;

	Transform* m_transform;

#pragma region Transform Shortcuts
public:
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
#pragma endregion
};

