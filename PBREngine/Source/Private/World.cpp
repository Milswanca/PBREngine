#include "EnginePCH.h"
#include "Engine.h"

World::World(ObjectInitData OI) : ObjectBase(OI)
{
	m_thisWorld = this;
	m_objectFactory = gEngine->CreateObject<ObjectFactory>(this);

	m_physicsManager = GetEngine()->GetPhysicsSystem();

	PxSceneDesc newSceneDesc(GetEngine()->GetPhysicsSystem()->GetPhysxToleranceScale());
	newSceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);

	PxDefaultCpuDispatcher* mCpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	newSceneDesc.cpuDispatcher = mCpuDispatcher;

	PxSimulationFilterShader gDefaultFilterShader = PxDefaultSimulationFilterShader;

	if (!newSceneDesc.filterShader)
	{
		newSceneDesc.filterShader = gDefaultFilterShader;
	}

	m_physxScene = m_physicsManager->GetPhysXAPI()->createScene(newSceneDesc);

	SpawnWorldObject<Camera>();
	SpawnWorldObject<TriangleDrawer>();
	SpawnWorldObject<Box>();

	m_physicsManager = GetEngine()->GetPhysicsSystem();
	PxMaterial* mat = m_physicsManager->GetPhysXAPI()->createMaterial(0.1f, 0.1f, 0.5f);

	PxPlaneGeometry planeGeom = PxPlaneGeometry();
	PxPlane()
	PxShape* shape = m_physicsManager->GetPhysXAPI()->createShape(planeGeom, *mat);
	m_physicsManager->GetPhysXAPI()->createRigidStatic(PxTransform(PxVec3(0.0f, 0, 0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f))));
}

World::~World()
{
	//delete m_objectFactory;
}

void World::Tick(float deltaTime)
{
	m_objectFactory->Tick(deltaTime);
	m_physxScene->simulate(deltaTime);

	while (!m_physxScene->fetchResults())
	{
		//Something here
	}
}

bool World::DestroyObject(WorldObject* object)
{
	if (!m_objectFactory) { return false; }

	m_objectFactory->DestroyObject(object);

	return true;
}

World* World::GetWorld() const
{
	return m_thisWorld;
}

PxScene* World::GetPhysXScene() const
{
	return m_physxScene;
}