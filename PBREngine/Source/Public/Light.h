#pragma once
#include "WorldObject.h"

class Light : public WorldObject
{
public:
	Light(ObjectInitData OI);
	~Light();

	virtual void Begin() override;
	virtual void Tick(float _deltaTime) override;

	virtual void AddDisplayProps();

	float GetIntensity();
	void SetIntensity(float _intensity);

	Vector3 GetColour();
	void SetColour(Vector3 _colour);

	unsigned int GetLightID() const;
	void SetLightID(unsigned int _id);

	Texture* GetShadowMap() const;

	virtual void SetShadowMaterialProperties();
	virtual enum ELightType GetLightType() const;
	virtual std::vector<std::string> GetShaderFiles() const;
	virtual unsigned int GetShadowMapSize() const;
	virtual enum EInitDepthType GetShadowMapType() const;
	virtual struct LightData GetLightData();

protected:
	class Texture* shadowTexture = nullptr;
	class RenderTarget* shadowRenderer = nullptr;
	class Material* shadowGenMaterial = nullptr;

private:
	Vector3 position;
	unsigned int m_lightID = 0;
	float m_intensity = 1;
	Vector3 m_colour;

	class RenderManager* m_renderManager = nullptr;
};

