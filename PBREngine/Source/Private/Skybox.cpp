#include "EnginePCH.h"
#include "Engine.h"

Skybox::Skybox(ObjectInitData OI) : WorldObject(OI)
{
}

void Skybox::Begin()
{
	WorldObject::Begin();

	m_boxMesh = AddComponent<StaticMeshComponent>();
	m_boxMesh->SetupAttachment(m_rootComponent);
	GLenum err = glGetError();
	m_boxMesh->SetStaticMesh(AssetManager::LoadAsset<StaticMesh>("Assets/StaticMeshes/Box.obj"));
	Material* mat = AssetManager::CreateDefaultMaterial(EMaterialType::MT_Skybox);
	m_boxMesh->SetMaterial(0, mat);
	err = glGetError();
	m_boxMesh->SetLocalScale(glm::vec3(1000, 1000, 1000));

	m_boxMesh->OnPreRender.Add(this, &Skybox::OnPreRender);
	m_boxMesh->OnPostRender.Add(this, &Skybox::OnPostRender);

	//SetSkyboxTexture(AssetManager::LoadAsset<CubeMap>("Assets/Skyboxes/MPCrimson/", "front.tga", "back.tga", "up.tga", "down.tga", "right.tga", "left.tga"));
	//SetSkyboxTexture(AssetManager::LoadAsset<CubeMap>("Assets/Skyboxes/Default/", "front.tga", "back.tga", "up.tga", "down.tga", "right.tga", "left.tga"));
	//SetSkyboxTexture(AssetManager::LoadAsset<CubeMap>(m_skyFiles[m_currentSky], 2048));
}

Skybox::~Skybox()
{
}

void Skybox::Tick(float DeltaTime)
{
	WorldObject::Tick(DeltaTime);
}

void Skybox::SetSkyboxTexture(CubeMap* texture)
{

	GLenum err = glGetError();
	m_environmentMap = texture;
	if (m_boxMesh->GetMaterial(0))
	{
		m_boxMesh->GetMaterial(0)->SetTextureValue("Skybox", m_environmentMap);
	}

	CreateIrradianceMap(64);
	m_boxMesh->GetMaterial(0)->SetTextureValue("MapIrradiance", m_irradianceMap);
	CreatePrefilteredEnvMap(512);
	m_boxMesh->GetMaterial(0)->SetTextureValue("MapPrefilter", m_prefilteredEnvMap);
	CreateBRDF(512);
	m_boxMesh->GetMaterial(0)->SetTextureValue("MapBRDF", m_brdf);
}

void Skybox::CreateIrradianceMap(unsigned int size)
{
	GLenum err = glGetError();

	//Create a cube camera
	CubeCamera* cubeCamera = gEngine->CreateObject<CubeCamera>(this);

	//Create a render target to render to textures
	RenderTarget* target = RenderTarget::Create(this, size, EInitDepthType::IDT_None);
	unsigned int attachID;
	m_irradianceMap = target->AddRenderTextureCube(attachID);

	// convert HDR equirectangular environment map to cubemap equivalent
	Material* mat = AssetManager::CreateCustomMaterial();
	mat->AttachShader(AssetManager::LoadAsset<Shader>("Assets/Materials/Default/createIrradianceMap.vert"));
	mat->AttachShader(AssetManager::LoadAsset<Shader>("Assets/Materials/Default/createIrradianceMap.frag"));
	mat->Compile();

	//Set default values
	mat->SetTextureValue("EnvironmentMap", m_environmentMap);
	mat->Use();

	target->SetAsActiveTarget();
	RenderManager* renderer = gEngine->GetRenderSystem();

	err = glGetError();

	for (unsigned int i = 0; i < 6; ++i)
	{
		ECubeMapSides side = static_cast<ECubeMapSides>(i);
		target->SetCubemapTargetFace(side);

		// attach it to the framebuffer as the first colour attachment
		// the FBO MUST still be bound
		target->ClearBuffers();

		//Draw cube
		renderer->RenderCube(cubeCamera->GetCamera(side), mat);
	}

	//Delete material
	mat->Unuse();
	gEngine->DestroyObject(mat);

	//Delete render target
	target->RemoveAsActiveTarget();
	gEngine->DestroyObject(target);

	//Delete render target
	gEngine->DestroyObject(cubeCamera);
}

void Skybox::CreatePrefilteredEnvMap(unsigned int size)
{
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	
	//Setup a cubemap camera
	CubeCamera* cubeCamera = gEngine->CreateObject<CubeCamera>(this);
	
	//Create a render target to render to texture
	RenderTarget* target = RenderTarget::Create(this, size, EInitDepthType::IDT_None);
	unsigned int attachID;
	m_prefilteredEnvMap = target->AddRenderTextureCube(attachID);

	m_prefilteredEnvMap->SetTextureParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_prefilteredEnvMap->GenerateMips();
	
	// convert HDR equirectangular environment map to cubemap equivalent
	Material* mat = AssetManager::CreateCustomMaterial();
	mat->AttachShader(AssetManager::LoadAsset<Shader>("Assets/Materials/Default/prefilterEnvMap.vert"));
	mat->AttachShader(AssetManager::LoadAsset<Shader>("Assets/Materials/Default/prefilterEnvMap.frag"));
	mat->Compile();
	
	//Set default values
	mat->SetTextureValue("EnvironmentMap", m_environmentMap);
	mat->Use();
	
	//Set as render target
	target->SetAsActiveTarget();

	RenderManager* renderer = gEngine->GetRenderSystem();

	unsigned int maxMipLevels = 5;
	for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
	{
		float roughness = (float)mip / (float)(maxMipLevels - 1);
		mat->SetFloatValue("roughness", roughness);

		// reisze framebuffer according to mip-level size.
		target->SetTargetMipLevel(mip);
	
		for (unsigned int i = 0; i < 6; ++i)
		{
			ECubeMapSides side = static_cast<ECubeMapSides>(i);
			
			target->SetCubemapTargetFace(side);

			// attach it to the framebuffer as the first colour attachment
			// the FBO MUST still be bound
			target->ClearBuffers();

			//Draw cube
			renderer->RenderCube(cubeCamera->GetCamera(side), mat);
		}
	}

	//Delete material
	mat->Unuse();
	gEngine->DestroyObject(mat);
	
	//Delete render target
	target->RemoveAsActiveTarget();
	gEngine->DestroyObject(target);

	//Delete Camera
	gEngine->DestroyObject(cubeCamera);
}

void Skybox::CreateBRDF(unsigned int size)
{
	//Create our texture cube
	m_brdf = gEngine->CreateObject<Texture2D>(this);

	//Create a render target to render to texture
	RenderTarget* target = RenderTarget::Create(this, size, EInitDepthType::IDT_None);
	unsigned int attachID;
	m_brdf = target->AddRenderTexture2D(attachID);

	// convert HDR equirectangular environment map to cubemap equivalent
	Material* mat = AssetManager::CreateCustomMaterial();
	mat->AttachShader(AssetManager::LoadAsset<Shader>("Assets/Materials/Default/createBRDF.vert"));
	mat->AttachShader(AssetManager::LoadAsset<Shader>("Assets/Materials/Default/createBRDF.frag"));
	mat->Compile();
	mat->Use();

	//Draw quad to render target
	target->SetAsActiveTarget();
	gEngine->GetRenderSystem()->RenderQuad(mat);
	
	target->RemoveAsActiveTarget();
	mat->Unuse();

	//Clean up
	gEngine->DestroyObject(target);
	gEngine->DestroyObject(mat);
}

CubeMap* Skybox::GetEnvironmentMap() const
{
	return m_environmentMap;
}

CubeMap* Skybox::GetIrradianceMap() const
{
	return m_irradianceMap;
}

CubeMap* Skybox::GetPrefilterEnvMap() const
{
	return m_prefilteredEnvMap;
}

Texture2D* Skybox::GetBRDF() const
{
	return m_brdf;
}

void Skybox::OnPreRender()
{
	glDisable(GL_CULL_FACE);
}

void Skybox::OnPostRender()
{
	glEnable(GL_CULL_FACE);
}