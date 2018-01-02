#pragma once
#include "Light.h"
class DirectionalLight : public Light
{
public:

	DirectionalLight(ObjectInitData OI);
	~DirectionalLight();

	virtual void Begin() override;
	virtual void Tick(float _deltaTime) override;

	virtual void AddDisplayProps() override;

	void SetDirection(Vector3 _direction);
	Vector3 GetDirection() const;

	glm::mat4 GetLightSpaceProjection();

	virtual void SetShadowMaterialProperties() override;
	virtual ELightType GetLightType() const override;
	virtual std::vector<std::string> GetShaderFiles() const override;
	virtual EInitDepthType GetShadowMapType() const override;
	virtual LightData GetLightData() override;

private:
	Vector3 m_direction;
};

