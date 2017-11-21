#pragma once
#include "ObjectBase.h"
#include "EngineBase.h"
#include "ComponentBase.h"

class WorldObject : public ObjectBase
{
public:
	WorldObject(ObjectInitData OI);
	~WorldObject();

	virtual void Begin();
	virtual void Tick(float deltaTime);
	virtual void Destroyed() override;

	void SetVisibility(bool visible);
	bool GetVisibility() const;

	class WorldComponent* GetRootComponent() const { return m_rootComponent; }
	virtual unsigned int GetRenderFlags() const { return ERenderFlags::RF_Important; }

	template<typename T>
	T* AddComponent()
	{
		//Error if T is not a component base
		static_assert(std::is_base_of<ComponentBase, T>::value, "Add component must have a template type of ComponentBase");

		T* newComp = gEngine->CreateObject<T>(this);
		m_components.push_back(newComp);
		newComp->Begin();
		return newComp;
	}

	template<typename T>
	T* GetComponent()
	{
		static_assert(std::is_base_of<ComponentBase, T>::value, "Get component must have a template type of ComponentBase");

		for (ComponentBase* i : m_components)
		{
			T* castedAsset = dynamic_cast<T*>(i);

			if (castedAsset)
			{
				return castedAsset;
			}
		}

		return nullptr;
	}

	class World* GetWorld() const override;


protected:
	bool bIsVisible;
	class WorldComponent* m_rootComponent;

private:
	class World* m_world;
	std::vector<ComponentBase*> m_components;

#pragma region Transform Shortcuts
public:
	void Rotate(float angle, glm::vec3 axis);

	void SetLocation(glm::vec3 location);
	void SetRotation(glm::quat rotation);
	void SetScale(glm::vec3 scale);

	glm::vec3 GetLocation();
	glm::quat GetRotation();
	glm::vec3 GetScale();
	class Transform* GetTransform() const;

	glm::vec3 GetRightVector();
	glm::vec3 GetUpVector();
	glm::vec3 GetForwardVector();
#pragma endregion
};

