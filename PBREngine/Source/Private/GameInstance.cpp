#include "EnginePCH.h"
#include "Engine.h"

GameInstance* GameInstance::instance = 0;

GameInstance::GameInstance(ObjectInitData OI) : ObjectBase(OI)
{

}

GameInstance::~GameInstance()
{

}

void GameInstance::DestroyInstance()
{
	if (instance)
	{
		delete instance;
	}
}