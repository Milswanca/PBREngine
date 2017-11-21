#pragma once
#include "WorldObject.h"

class TriangleDrawer : public WorldObject
{
public:
	TriangleDrawer(ObjectInitData OI);
	~TriangleDrawer();

	virtual void Begin() override;
	virtual void Tick(float deltaTime) override;

private:
	float m_timeBetweenLights = 3.0f;
	float m_lightTimer;

	class RigidbodyComponent* m_rigidBody = nullptr;

	WorldComponent* m_lightRoot = nullptr;
	float RandomFloat(float a, float b);
	void SpawnLight();
	std::vector<WorldObject*> lights;

	class Skybox* GetCurrentSky() const;
	class StaticMesh* GetCurrentStaticMesh() const;

	std::vector<class Skybox*> skys;
	class StaticMeshComponent* meshComponent = nullptr;

	int m_currentSky = 0;
	std::vector<const char*> m_skyFiles;

	int m_currentMesh = 0;
	std::vector<const char*> m_meshFiles;
	std::vector <glm::vec3> m_scales;
	std::vector <bool> m_recalculateNormals;
	std::vector<class StaticMesh*> meshes;

	void NextSky();
	void PreviousSky();

	void NextMesh();
	void PreviousMesh();

	glm::vec3 albedo;
	float metallic;
	float roughness;

	TwBar* materialUI;
};

