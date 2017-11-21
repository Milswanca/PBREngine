#include "EnginePCH.h"
#include "Engine.h"

ComponentBase::ComponentBase(ObjectInitData OI) : ObjectBase(OI)
{

}

ComponentBase::~ComponentBase()
{

}

void ComponentBase::Begin()
{

}

void ComponentBase::Tick(float deltaTime)
{

}

void ComponentBase::End()
{

}

WorldObject* ComponentBase::GetOwner()
{
	return m_owner;
}

ESystemType ComponentBase::GetSystemType() const
{
	return ESystemType::ST_None;
}