#pragma once
#include "WorldObject.h"

class CubeCamera : public WorldObject
{
public:
	CubeCamera(ObjectInitData OI);
	~CubeCamera();

	virtual void Tick(float _deltaTime) override;

	class CameraComponent* GetCamera(ECubeMapSides side) const;

	void UpdateProjections();

private:
	class CameraComponent* m_cameras[6];
};

