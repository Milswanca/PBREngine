#pragma once
#include "WorldComponent.h"

class CameraComponent : public WorldComponent
{
public:
	CameraComponent(ObjectInitData OI);
	~CameraComponent();

	virtual void Begin() override;
	virtual void Tick(float deltaTime) override;

	void LookAt(glm::vec3 location, glm::vec3 up = glm::vec3(0, 1, 0));
	void SetPerspective(float AspectRatio, float FOV, float NearClip, float FarClip);

	void UpdateProjectionView();

	glm::mat4 GetProjectionView() const;
	glm::mat4 GetView() const;
	glm::mat4 GetProjection() const;

private:
	glm::mat4 m_projection;
	glm::mat4 m_view;
	glm::mat4 m_projectionView;

	float m_aspectRatio;
	float m_fieldOfView;

	float m_nearClip;
	float m_farClip;
};

