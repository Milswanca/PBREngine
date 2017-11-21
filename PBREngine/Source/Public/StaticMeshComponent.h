#pragma once
#include "WorldComponent.h"

class StaticMeshComponent : public WorldComponent
{
public:
	StaticMeshComponent(ObjectInitData OI);
	~StaticMeshComponent();

	virtual void Tick(float deltaTime);
	virtual void SetStaticMesh(class StaticMesh* staticMesh);

	virtual void SetMaterial(int materialNum, class Material* material);
	virtual Material* GetMaterial(int index) const;

	void RecalculateNormals();
	void RecalculateTangents();

	virtual void Render(class CameraComponent* camera);

	unsigned int GetNumMaterials() const;

	Delegate<void> OnPreRender;
	Delegate<void> OnPostRender;

private:
	class StaticMesh* m_staticMesh = nullptr;
	class RenderManager* m_renderManager = nullptr;
	std::vector<class Material*> m_materials;
};

