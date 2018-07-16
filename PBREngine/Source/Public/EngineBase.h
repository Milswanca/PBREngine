#pragma once
#include "ObjectBase.h"
#include "ImguiFunctions.h"

class EngineBase : public ObjectBase
{
public:
	EngineBase(ObjectInitData OI);
	~EngineBase();

	virtual int Init();
	virtual void EngineLoop();
	virtual GLFWwindow* CreateWindow(unsigned int width, unsigned int height);
	virtual void Shutdown();

	class World* CurrentWorld;

	time_t CurrentTime;
	time_t LastTime;

	class RenderManager* GetRenderSystem();
	class GLFWwindow* GetCurrentWindow() const;

	template<typename T>
	T* CreateObject(ObjectBase* outer)
	{
		World* world = nullptr;

		if (outer)
		{
			world = outer->GetWorld();
		}

		T* newObj = new T(ObjectInitData(this, world, outer));
		m_allObjects.push_back(static_cast<ObjectBase*>(newObj));

		return newObj;
	}

	void DestroyObject(class ObjectBase* obj)
	{
		auto iter = std::find(m_allObjects.begin(), m_allObjects.end(), obj);

		if (iter != m_allObjects.end())
		{
			m_allObjects.erase(iter);
			obj->Destroyed();
			delete obj;
		}
	}

private:
	std::map<ESystemType, class SystemBase*> m_systems;
	RenderManager* m_renderManager = nullptr;
	std::vector<ObjectBase*> m_allObjects;

	bool bWantsToClose;
	GLFWwindow* m_currentWindow;
};

