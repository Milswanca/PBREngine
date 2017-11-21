#include "EnginePCH.h"
#include "Engine.h"

std::map<EKeyType, KeyData> Input::m_keyData;
GLFWwindow* Input::m_window = nullptr;
glm::vec2 Input::m_mouseCurrent = glm::vec2(0);
glm::vec2 Input::m_mouseLast = glm::vec2(0);
glm::vec2 Input::m_mouseScrollDelta = glm::vec2(0);
bool Input::m_cursorVisibility = true;

void Input::Init(GLFWwindow* _window)
{
	m_keyData[EKeyType::KEY_0] = KeyData(GLFW_KEY_0, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_1] = KeyData(GLFW_KEY_1, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_2] = KeyData(GLFW_KEY_2, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_3] = KeyData(GLFW_KEY_3, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_4] = KeyData(GLFW_KEY_4, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_5] = KeyData(GLFW_KEY_5, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_6] = KeyData(GLFW_KEY_6, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_7] = KeyData(GLFW_KEY_7, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_8] = KeyData(GLFW_KEY_8, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_9] = KeyData(GLFW_KEY_9, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_A] = KeyData(GLFW_KEY_A, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_B] = KeyData(GLFW_KEY_B, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_C] = KeyData(GLFW_KEY_C, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_D] = KeyData(GLFW_KEY_D, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_E] = KeyData(GLFW_KEY_E, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_F] = KeyData(GLFW_KEY_F, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_G] = KeyData(GLFW_KEY_G, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_H] = KeyData(GLFW_KEY_H, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_I] = KeyData(GLFW_KEY_I, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_J] = KeyData(GLFW_KEY_J, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_K] = KeyData(GLFW_KEY_K, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_L] = KeyData(GLFW_KEY_L, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_M] = KeyData(GLFW_KEY_M, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_N] = KeyData(GLFW_KEY_N, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_O] = KeyData(GLFW_KEY_O, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_P] = KeyData(GLFW_KEY_P, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_Q] = KeyData(GLFW_KEY_Q, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_R] = KeyData(GLFW_KEY_R, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_S] = KeyData(GLFW_KEY_S, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_T] = KeyData(GLFW_KEY_T, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_U] = KeyData(GLFW_KEY_U, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_V] = KeyData(GLFW_KEY_V, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_W] = KeyData(GLFW_KEY_W, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_X] = KeyData(GLFW_KEY_X, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_Y] = KeyData(GLFW_KEY_Y, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_Z] = KeyData(GLFW_KEY_Z, EKeyState::KS_None, EButtonType::BT_KEY);
	m_keyData[EKeyType::KEY_MOUSE_LEFT] = KeyData(GLFW_MOUSE_BUTTON_LEFT, EKeyState::KS_None, EButtonType::BT_MOUSE);
	m_keyData[EKeyType::KEY_MOUSE_MIDDLE] = KeyData(GLFW_MOUSE_BUTTON_MIDDLE, EKeyState::KS_None, EButtonType::BT_MOUSE);
	m_keyData[EKeyType::KEY_MOUSE_RIGHT] = KeyData(GLFW_MOUSE_BUTTON_RIGHT, EKeyState::KS_None, EButtonType::BT_MOUSE);

	m_window = _window;
	m_mouseCurrent = GetGLMousePos();
	m_mouseLast = GetMousePosition();

	glfwSetScrollCallback(m_window, &Input::MouseScrollCallback);
}

void Input::PreProcess()
{
	for (int i = 0; i < (int)EKeyType::KEY_ANY; ++i)
	{
		UpdateKey((EKeyType)i);
	}

	//Update mouse
	m_mouseCurrent = GetGLMousePos();
}

void Input::PostProcess()
{
	m_mouseLast = GetMousePosition();
	m_mouseScrollDelta = glm::vec2(0);
}

void Input::UpdateKey(EKeyType _key)
{
	int keyID = ConvertKey(_key);

	EKeyState lastState = m_keyData[_key].KeyState;

	GLenum keyState;

	switch (m_keyData[_key].ButtonType)
	{
	case EButtonType::BT_KEY:
		keyState = glfwGetKey(m_window, keyID);
		break;
	case EButtonType::BT_MOUSE:
		keyState = glfwGetMouseButton(m_window, keyID);
		break;
	}

	//None to pressed
	if (keyState == GLFW_PRESS)
	{
		if (lastState == EKeyState::KS_None || lastState == EKeyState::KS_Released)
		{
			m_keyData[_key].KeyState = EKeyState::KS_Pressed;
			return;
		}
		else if (lastState == EKeyState::KS_Pressed)
		{
			m_keyData[_key].KeyState = EKeyState::KS_Down;
		}
	}
	//Down or pressed to released
	else if (keyState == GLFW_RELEASE)
	{
		if ((lastState == EKeyState::KS_Down || lastState == EKeyState::KS_Pressed))
		{
			m_keyData[_key].KeyState = EKeyState::KS_Released;
			return;
		}
		else if (lastState == EKeyState::KS_Released)
		{
			m_keyData[_key].KeyState = EKeyState::KS_None;
			return;
		}
	}
}

bool Input::IsKeyDown(EKeyType _key)
{
	return m_keyData[_key].KeyState == EKeyState::KS_Down || m_keyData[_key].KeyState == EKeyState::KS_Pressed;
}

bool Input::IsKeyPressed(EKeyType _key)
{
	return m_keyData[_key].KeyState == EKeyState::KS_Pressed;
}

bool Input::IsKeyReleased(EKeyType _key)
{
	return m_keyData[_key].KeyState == EKeyState::KS_Released;
}

int Input::ConvertKey(EKeyType _key)
{
	return m_keyData[_key].ID;
}

glm::vec2 Input::GetMouseDelta()
{
	return GetMousePosition() - m_mouseLast;
}

void Input::SetMousePosition(glm::vec2 _pos)
{
	m_mouseLast = _pos;
	glfwSetCursorPos(m_window, _pos.x, _pos.y);
	m_mouseCurrent = _pos;
}

glm::vec2 Input::GetMousePosition()
{
	return m_mouseCurrent;
}

glm::vec2 Input::GetMouseScrollDelta()
{
	return m_mouseScrollDelta;
}

void Input::SetCursorVisibility(bool _visibility)
{
	m_cursorVisibility = _visibility;

	if (m_cursorVisibility)
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
}

void Input::MouseScrollCallback(GLFWwindow* _window, double _xoffset, double _yoffset)
{
	m_mouseScrollDelta = glm::vec2(_xoffset, _yoffset);
}

glm::vec2 Input::GetGLMousePos()
{
	double posx = 0;
	double posy = 0;
	glfwGetCursorPos(m_window, &posx, &posy);

	return glm::vec2(posx, posy);
}