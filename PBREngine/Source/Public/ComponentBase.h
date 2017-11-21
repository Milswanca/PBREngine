#pragma once
#include "ObjectBase.h"

class ComponentBase : public ObjectBase
{
public:
	ComponentBase(ObjectInitData OI);
	~ComponentBase();

	virtual ESystemType GetSystemType() const;

	virtual void Begin();
	virtual void Tick(float deltaTime);
	virtual void End();

	class WorldObject* GetOwner();

private:
	class WorldObject* m_owner;
};

