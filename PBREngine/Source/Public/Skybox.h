#pragma once
#include "WorldObject.h"

class Skybox : public WorldObject
{
public:
	Skybox(ObjectInitData OI);
	~Skybox();

	virtual void Begin() override;
	virtual void Tick(float DeltaTime) override;

	virtual unsigned int GetRenderFlags() const override { return ERenderFlags::RF_Important | ERenderFlags::RF_IsSkybox; }

	void SetSkyboxTexture(class CubeMap* texture);

	CubeMap* GetIrradianceMap() const;
	CubeMap* GetEnvironmentMap() const;
	CubeMap* GetPrefilterEnvMap() const;
	class Texture2D* GetBRDF() const;

	void OnPreRender();
	void OnPostRender();

private:
	void CreateIrradianceMap(unsigned int size);
	void CreatePrefilteredEnvMap(unsigned int size);
	void CreateBRDF(unsigned int size);

	class CubeMap* m_environmentMap;
	class CubeMap* m_irradianceMap;
	class CubeMap* m_prefilteredEnvMap;
	class Texture2D* m_brdf;

	class StaticMeshComponent* m_boxMesh;

	unsigned int test;
};

