#include "EnginePCH.h"
#include "Engine.h"

CubeCamera::CubeCamera(ObjectInitData OI) : WorldObject(OI)
{
	for (int i = 0; i < 6; i++)
	{
		m_cameras[i] = AddComponent<CameraComponent>();
		m_cameras[i]->SetupAttachment(m_rootComponent);

		m_cameras[i]->SetPerspective(1.0f, glm::radians(90.0f), 0.1f, 25.0f);
	}

	UpdateProjections();
}

CubeCamera::~CubeCamera()
{
}

void CubeCamera::Tick(float _deltaTime)
{
	WorldObject::Tick(_deltaTime);
	UpdateProjections();
}

void CubeCamera::UpdateProjections()
{
	m_cameras[0]->LookAt(m_cameras[0]->GetWorldLocation() + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	m_cameras[1]->LookAt(m_cameras[1]->GetWorldLocation() + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	m_cameras[2]->LookAt(m_cameras[2]->GetWorldLocation() + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	m_cameras[3]->LookAt(m_cameras[3]->GetWorldLocation() + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	m_cameras[4]->LookAt(m_cameras[4]->GetWorldLocation() + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	m_cameras[5]->LookAt(m_cameras[5]->GetWorldLocation() + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
}

CameraComponent* CubeCamera::GetCamera(ECubeMapSides side) const
{
	return m_cameras[(int)side];
}