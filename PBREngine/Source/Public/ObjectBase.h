#pragma once
#include "Serializer.h"

struct ObjectInitData
{
public:
	class EngineBase* Engine;
	class World* World;
	class ObjectBase* Outer;

	ObjectInitData() : Engine(nullptr), World(nullptr) {}
	ObjectInitData(class EngineBase* engine, class ObjectBase* outer) : Engine(engine), World(nullptr), Outer(outer) {}
	ObjectInitData(class EngineBase* engine, class World* world, class ObjectBase* outer) : Engine(engine), World(world), Outer(outer) {}
};

class ObjectBase
{
public:
	ObjectBase(ObjectInitData OI);
	virtual ~ObjectBase();

	virtual class World* GetWorld() const { return nullptr; }
	virtual void PostLoad() {}
	virtual void Destroyed() {}

	//Implement these to support serialization
	virtual void Serialize(std::vector<SerializedData>& _outData) {}
	virtual void Deserialize(std::vector<SerializedData> _fromData) {}

	class GameInstance* GetGameInstance() const;
	class EngineBase* GetEngine() const;

	void MarkAsDirty() { bIsDirty = true; }

protected:
	bool bIsDirty = false;

	class EngineBase* gEngine = nullptr;

private:

	friend class EngineBase;
};

