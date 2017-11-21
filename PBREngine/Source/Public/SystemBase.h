#pragma once
#include "ObjectBase.h"

class SystemBase : public ObjectBase
{
public:
	SystemBase(ObjectInitData OI) : ObjectBase(OI) {}
	~SystemBase() {}

	virtual void Init() = 0;
	virtual void Update(float DeltaTime) = 0;
	virtual void Deinit() = 0;
};