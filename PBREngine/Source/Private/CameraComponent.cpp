#include "EnginePCH.h"
#include "Engine.h"

CameraComponent::CameraComponent(ObjectInitData OI) : WorldComponent(OI)
{
	m_aspectRatio = 4.0f / 3.0f;
	m_fieldOfView = 45.0f;
	m_nearClip = 0.1f;
	m_farClip = 10000.0f;

	LookAt(GetWorldLocation() + (GetForwardVector() * 3.0f), Vector3(0, 1, 0));
	SetPerspective(m_aspectRatio, m_fieldOfView, m_nearClip, m_farClip);
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::Begin()
{
	ComponentBase::Begin();
}

void CameraComponent::Tick(float deltaTime)
{
	ComponentBase::Tick(deltaTime);

	LookAt(GetWorldLocation() + (GetForwardVector() * 3.0f), Vector3(0, 1, 0));
}

void CameraComponent::LookAt(Vector3 location, Vector3 up)
{
	m_view = glm::lookAt(GetWorldLocation(), location, up);
	UpdateProjectionView();
}

void CameraComponent::SetPerspective(float aspectRatio, float FOV, float nearClip, float farClip)
{
	m_projection = glm::perspective(FOV, aspectRatio, nearClip, farClip);

	m_aspectRatio = aspectRatio;
	m_fieldOfView = FOV;
	m_nearClip = nearClip;
	m_farClip = farClip;

	UpdateProjectionView();
}

void CameraComponent::UpdateProjectionView()
{
	m_projectionView = m_projection * m_view; // Remember, matrix multiplication is the other way around
}

glm::mat4 CameraComponent::GetProjectionView() const
{
	return m_projectionView;
}

glm::mat4 CameraComponent::GetView() const
{
	return m_view;
}

glm::mat4 CameraComponent::GetProjection() const
{
	return m_projection;
}