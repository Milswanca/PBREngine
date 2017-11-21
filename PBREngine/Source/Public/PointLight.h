#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	PointLight(ObjectInitData OI);
	~PointLight();

	virtual void Begin() override;

	virtual void AddDisplayProps() override;

	float GetRadius() const;
	void SetRadius(float _radius);

	virtual void SetShadowMaterialProperties() override;
	virtual enum ELightType GetLightType() const override;
	virtual std::vector<std::string> GetShaderFiles() const override;
	virtual enum EInitDepthType GetShadowMapType() const override;
	virtual struct LightData GetLightData() override;

private:
	class CubeCamera* m_cubeProjections = nullptr;

	float m_radius = 1;
};

