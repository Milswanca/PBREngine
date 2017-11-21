#include "EnginePCH.h"
#include "Engine.h"

World::World(ObjectInitData OI) : ObjectBase(OI)
{
	m_thisWorld = this;
	m_objectFactory = gEngine->CreateObject<ObjectFactory>(this);

	m_physicsManager = GetEngine()->GetPhysicsSystem();

	PxSceneDesc newSceneDesc(GetEngine()->GetPhysicsSystem()->GetPhysxToleranceScale());
	newSceneDesc.gravity = PxVec3(0.0f, -190.8f, 0.0f);

	PxDefaultCpuDispatcher* mCpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	newSceneDesc.cpuDispatcher = mCpuDispatcher;

	PxSimulationFilterShader gDefaultFilterShader = PxDefaultSimulationFilterShader;

	if (!newSceneDesc.filterShader)
	{
		newSceneDesc.filterShader = gDefaultFilterShader;
	}

	m_physxScene = m_physicsManager->GetPhysXAPI()->createScene(newSceneDesc);

	m_skybox = SpawnWorldObject<Skybox>();
	m_skybox->SetSkyboxTexture(CubeMap::Create(this, "Assets/Skyboxes/EquirectangularSkys/Alexs_Apt_8k.jpg"));

	m_physicsManager = GetEngine()->GetPhysicsSystem();
	PxMaterial* mat = m_physicsManager->GetPhysXAPI()->createMaterial(1.0f, 1.0f, 0.8f);

	PxPlane plane = PxPlane(0.0f, 1.0f, 0.0f, -30.0f);
	PxActor* actor = PxCreatePlane(*m_physicsManager->GetPhysXAPI(), plane, *mat);
	m_physxScene->addActor(*actor);

	SpawnWorldObject<DirectionalLight>();

	SpawnWorldObject<Camera>();
	for (int i = 0; i < 10; ++i)
	{
		Box* box = SpawnWorldObject<Box>();
		box->GetStaticMesh()->GetMaterial(0)->SetTextureValue("MapIrradiance", GetSkybox()->GetIrradianceMap());
		box->GetStaticMesh()->GetMaterial(0)->SetTextureValue("MapPrefilteredEnv", GetSkybox()->GetPrefilterEnvMap());
		box->GetStaticMesh()->GetMaterial(0)->SetTextureValue("MapBRDF", GetSkybox()->GetBRDF());
		m_boxes.push_back(box);
	}
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

	ImGui::Begin("Dragon Properties");
	ImGui::SliderFloat("Metallic", &metallic, 0, 1);
	ImGui::SliderFloat("Roughness", &roughness, 0, 1);
	ImGui::ColorEdit3("Albedo", (float*)&albedo);
	ImGui::End();

	for (Box* i : m_boxes)
	{
		i->GetStaticMesh()->GetMaterial(0)->SetFloatValue("Metallic", metallic);
		i->GetStaticMesh()->GetMaterial(0)->SetFloatValue("Roughness", roughness);
		i->GetStaticMesh()->GetMaterial(0)->SetVector3Value("Albedo", albedo);
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

Skybox* World::GetSkybox() const
{
	return m_skybox;
}