#include "EnginePCH.h"
#include "EngineBase.h"
//#include <vld.h>

int main()
{
	EngineBase* engine = new EngineBase(ObjectInitData());

	engine->Init();
	engine->EngineLoop();

	delete engine;

	return 0;
}