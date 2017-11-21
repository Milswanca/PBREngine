#pragma once
#include "WorldObject.h"

class Camera : public WorldObject
{
public:
	Camera(ObjectInitData OI);
	~Camera();

	virtual void Begin() override;
	virtual void Tick(float deltaTime) override;

	class CameraComponent* GetCameraComponent() const;

private:
	void IncreaseSpeed();
	void DecreaseSpeed();

	glm::vec3 lastMouse;
	glm::vec3 eulerLookAt;
	float sensitivity = 0.3f;

	float scrollZoomAmount = 5.0f;

	float speed = 2.0f;

	float speedMin = 0.1f;
	float speedMax = 4.0f;
	float speedScrollIncrement = 0.1f;

	bool cursorDownLastFrame = false;

	class CameraComponent* m_camera;
};

