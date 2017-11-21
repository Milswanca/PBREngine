#include "EnginePCH.h"
#include "Engine.h"

ObjectBase::ObjectBase(ObjectInitData OI)
{
	gEngine = OI.Engine;
}

ObjectBase::~ObjectBase()
{
}

GameInstance* ObjectBase::GetGameInstance() const
{
	return GameInstance::Instance();
}

EngineBase* ObjectBase::GetEngine() const
{
	return gEngine;
}