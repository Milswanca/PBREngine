#include "EnginePCH.h"
#include "Engine.h"

TriangleDrawer::TriangleDrawer(ObjectInitData OI) : WorldObject(OI)
{

}

TriangleDrawer::~TriangleDrawer()
{
}

void TriangleDrawer::Begin()
{
	WorldObject::Begin();

	std::srand(time(0));

	m_lightRoot = AddComponent<WorldComponent>();
	m_lightRoot->SetupAttachment(GetRootComponent());

	SpawnLight();

	m_skyFiles.push_back("Assets/Skyboxes/EquirectangularSkys/Alexs_Apt_8k.jpg");
	m_skyFiles.push_back("Assets/Skyboxes/EquirectangularSkys/Arches_E_PineTree_8k.jpg");
	m_skyFiles.push_back("Assets/Skyboxes/EquirectangularSkys/WinterForest_8k.jpg");

	m_meshFiles.push_back("Assets/StaticMeshes/Render_Ball.obj");
	m_meshFiles.push_back("Assets/StaticMeshes/sphere.obj");
	////m_meshFiles.push_back("Assets/StaticMeshes/testCow.obj");
	m_meshFiles.push_back("Assets/StaticMeshes/wc1.obj");
	m_meshFiles.push_back("Assets/StaticMeshes/StanfordDragon/stanford-dragon.obj");
	m_meshFiles.push_back("Assets/StaticMeshes/Sniper/sniperGun.obj");
	m_meshFiles.push_back("Assets/StaticMeshes/Nats_Skateboard.obj");

	m_recalculateNormals.push_back(false);
	m_recalculateNormals.push_back(false);
	m_recalculateNormals.push_back(true);
	m_recalculateNormals.push_back(false);
	m_recalculateNormals.push_back(false);
	m_recalculateNormals.push_back(false);

	m_scales.push_back(glm::vec3(0.1f, 0.1f, 0.1f));
	m_scales.push_back(glm::vec3(10, 10, 10));
	m_scales.push_back(glm::vec3(0.1f, 0.1f, 0.1f));
	m_scales.push_back(glm::vec3(2, 2, 2));
	m_scales.push_back(glm::vec3(7, 7, 7));
	m_scales.push_back(glm::vec3(10, 10, 10));
	GLenum err = glGetError();

	//Create meshes
	for (int i = 0; i < m_meshFiles.size(); ++i)
	{
		//meshes.push_back(AssetManager::LoadAsset<StaticMesh>(m_meshFiles[i], m_recalculateNormals[i]));
	}

	meshComponent = AddComponent<StaticMeshComponent>();
	meshComponent->SetupAttachment(GetRootComponent());
	//meshComponent->SetStaticMesh(AssetManager::LoadAsset<StaticMesh>("Assets/StaticMeshes/Render_ball.obj", false));
	//meshComp->RecalculateNormals();
	//meshComponent->SetRelativeLocation(glm::vec3(0, 3, 0));
	
	//Create a skybox
	for (int i = 0; i < m_skyFiles.size(); ++i)
	{
		skys.push_back(GetWorld()->SpawnWorldObject<Skybox>());
		skys[i]->SetSkyboxTexture(CubeMap::Create(this, m_skyFiles[i], 4096));
		skys[i]->SetVisibility(false);
	}

	GetCurrentSky()->SetVisibility(true);

	meshComponent->SetStaticMesh(AssetManager::LoadAsset<StaticMesh>(m_meshFiles[m_currentMesh], m_recalculateNormals[m_currentMesh]));
	for (int i = 0; i < meshComponent->GetNumMaterials(); ++i)
	{
		if (!meshComponent->GetMaterial(i)) { continue; }

		meshComponent->GetMaterial(i)->SetTextureValue("MapIrradiance", GetCurrentSky()->GetIrradianceMap());
		meshComponent->GetMaterial(i)->SetTextureValue("MapPrefilteredEnv", GetCurrentSky()->GetPrefilterEnvMap());
		meshComponent->GetMaterial(i)->SetTextureValue("MapBRDF", GetCurrentSky()->GetBRDF());
	}

	meshComponent->SetLocalScale(m_scales[m_currentMesh]);

	metallic = 0.5f;
	albedo = glm::vec3(0.3, 0.3, 0.3);
	roughness = 0.1f;
}

float TriangleDrawer::RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

void TriangleDrawer::SpawnLight()
{
	DirectionalLight* light = GetWorld()->SpawnWorldObject<DirectionalLight>();
	light->GetRootComponent()->SetupAttachment(m_lightRoot);
	light->SetColour(glm::vec3(RandomFloat(0.2, 1), RandomFloat(0.2, 1), RandomFloat(0.2, 1)));
	light->SetIntensity(0.3f);
	glm::vec3 spawnRadius = glm::vec3(RandomFloat(-1, 1), 0, RandomFloat(-1, 1));
	spawnRadius = glm::normalize(spawnRadius);
	spawnRadius *= RandomFloat(-50.0f, 50.0f);
	light->GetRootComponent()->SetLocalScale(glm::vec3(spawnRadius.x, RandomFloat(10, 15), spawnRadius.z));
	lights.push_back(light);
}

void TriangleDrawer::Tick(float deltaTime)
{
	WorldObject::Tick(deltaTime);

	m_lightTimer -= deltaTime;

	if (m_lightTimer < 0.0f)
	{
		//SpawnLight();
		m_lightTimer = m_timeBetweenLights;
	}
	
	//m_lightRoot->Rotate(1.0f * deltaTime, glm::vec3(0, 1, 0));

	ImGui::Begin("Dragon Properties");
	ImGui::SliderFloat("Metallic", &metallic, 0, 1);
	ImGui::SliderFloat("Roughness", &roughness, 0, 1);
	ImGui::ColorEdit3("Albedo", (float*)&albedo);
	if (ImGui::Button("Next Sky")) NextSky();
	if (ImGui::Button("Previous Sky")) PreviousSky();
	if (ImGui::Button("Next Mesh")) NextMesh();
	if (ImGui::Button("Previous Mesh")) PreviousMesh();
	ImGui::End();

	for (int i = 0; i < meshComponent->GetNumMaterials(); ++i)
	{
		if (!meshComponent->GetMaterial(i)) { continue; }
	
		meshComponent->GetMaterial(i)->SetFloatValue("Metallic", metallic);
		meshComponent->GetMaterial(i)->SetFloatValue("Roughness", roughness);
		meshComponent->GetMaterial(i)->SetVector3Value("Albedo", albedo);
		meshComponent->GetMaterial(i)->SetTextureValue("MapIrradiance", GetCurrentSky()->GetIrradianceMap());
		meshComponent->GetMaterial(i)->SetTextureValue("MapPrefilteredEnv", GetCurrentSky()->GetPrefilterEnvMap());
		meshComponent->GetMaterial(i)->SetTextureValue("MapBRDF", GetCurrentSky()->GetBRDF());
	}
}

void TriangleDrawer::NextSky()
{
	GetCurrentSky()->SetVisibility(false);

	m_currentSky++;

	if (m_currentSky > m_skyFiles.size() - 1)
	{
		m_currentSky = 0;
	}

	GetCurrentSky()->SetVisibility(true);
}

void TriangleDrawer::PreviousSky()
{
	GetCurrentSky()->SetVisibility(false);

	m_currentSky--;

	if (m_currentSky < 0)
	{
		m_currentSky = m_skyFiles.size() - 1;
	}

	GetCurrentSky()->SetVisibility(true);
}

Skybox* TriangleDrawer::GetCurrentSky() const
{
	return skys[m_currentSky];
}

void TriangleDrawer::NextMesh()
{
	m_currentMesh++;

	if (m_currentMesh > m_meshFiles.size() - 1)
	{
		m_currentMesh = 0;
	}

	meshComponent->SetStaticMesh(AssetManager::LoadAsset<StaticMesh>(m_meshFiles[m_currentMesh], m_recalculateNormals[m_currentMesh]));
	meshComponent->SetLocalScale(m_scales[m_currentMesh]);
}

void TriangleDrawer::PreviousMesh()
{
	m_currentMesh--;

	if (m_currentMesh < 0)
	{
		m_currentMesh = m_meshFiles.size() - 1;
	}

	meshComponent->SetStaticMesh(AssetManager::LoadAsset<StaticMesh>(m_meshFiles[m_currentMesh], m_recalculateNormals[m_currentMesh]));
	meshComponent->SetLocalScale(m_scales[m_currentMesh]);
}

StaticMesh* TriangleDrawer::GetCurrentStaticMesh() const
{
	return meshes[m_currentMesh];
}