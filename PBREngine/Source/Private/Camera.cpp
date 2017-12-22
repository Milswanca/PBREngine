#include "EnginePCH.h"
#include "Engine.h"

Camera::Camera(ObjectInitData OI) : WorldObject(OI)
{

}

Camera::~Camera()
{

}

void Camera::Begin()
{
	WorldObject::Begin();

	GetGameInstance()->MainCamera = this;

	m_camera = AddComponent<CameraComponent>();
	m_camera->SetupAttachment(m_rootComponent);
	//m_camera->SetLocalLocation(Vector3(, 0, 0));
}

void Camera::Tick(float deltaTime)
{
	WorldObject::Tick(deltaTime);

	if (Input::IsKeyDown(EKeyType::KEY_MOUSE_RIGHT))
	{
		if (!cursorDownLastFrame)
		{
			Input::SetMousePosition(glm::vec2(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5f));
			cursorDownLastFrame = true;
		}

		Input::SetCursorVisibility(false);
		
		float scrollDelta = Input::GetMouseScrollDelta().y;

		if (scrollDelta > 0.0f)
		{
			IncreaseSpeed();
		}
		else if (scrollDelta < 0.0f)
		{
			DecreaseSpeed();
		}

		if (Input::IsKeyDown(EKeyType::KEY_W))
		{
			SetLocation(GetLocation() + GetForwardVector() * speed);
		}

		if (Input::IsKeyDown(EKeyType::KEY_S))
		{
			SetLocation(GetLocation() - GetForwardVector() * speed);
		}

		if (Input::IsKeyDown(EKeyType::KEY_A))
		{
			SetLocation(GetLocation() + GetRightVector() * speed);
		}

		if (Input::IsKeyDown(EKeyType::KEY_D))
		{
			Vector3 right = GetRightVector();
			SetLocation(GetLocation() - (right * speed));
		}

		if (Input::IsKeyDown(EKeyType::KEY_E))
		{
			SetLocation(GetLocation() + Vector3(0, 1, 0) * speed);
		}

		if (Input::IsKeyDown(EKeyType::KEY_Q))
		{
			SetLocation(GetLocation() - Vector3(0, 1, 0) * speed);
		}

		glm::vec2 mouseDelta = Input::GetMouseDelta();

		eulerLookAt.x -= mouseDelta.x * sensitivity;
		eulerLookAt.y += mouseDelta.y * sensitivity;

		eulerLookAt.y = glm::clamp(eulerLookAt.y, -89.0f, 89.0f);

		SetRotation(glm::vec3(glm::radians(eulerLookAt.y), glm::radians(eulerLookAt.x), 0.0f));

		Input::SetMousePosition(glm::vec2(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5f));
	}
	else
	{
		Input::SetCursorVisibility(true);
		cursorDownLastFrame = false;

		float scrollDelta = Input::GetMouseScrollDelta().y;

		SetLocation(GetLocation() + (GetForwardVector() * scrollZoomAmount * scrollDelta));
	}

	ImGui::Begin("Debug");
	ImGui::LabelText(std::to_string(eulerLookAt.x).c_str(), "");
	ImGui::LabelText(std::to_string(eulerLookAt.y).c_str(), "");
	ImGui::End();
}

CameraComponent* Camera::GetCameraComponent() const
{
	return m_camera;
}

void Camera::IncreaseSpeed()
{
	float increment = (speedMax - speedMin) * speedScrollIncrement;
	speed += increment;
	speed = glm::clamp(speed, speedMin, speedMax);
}

void Camera::DecreaseSpeed()
{
	float increment = (speedMax - speedMin) * speedScrollIncrement;
	speed -= increment;
	speed = glm::clamp(speed, speedMin, speedMax);
}