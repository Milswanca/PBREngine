#pragma once

enum EKeyType
{
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_0,
	KEY_MOUSE_LEFT,
	KEY_MOUSE_RIGHT,
	KEY_MOUSE_MIDDLE,
	KEY_ANY
};

enum EKeyState
{
	KS_Down,
	KS_Pressed,
	KS_Released,
	KS_None
};

enum EButtonType
{
	BT_KEY,
	BT_MOUSE
};

struct KeyData
{
public:
	int ID = -1;
	EKeyState KeyState;
	EButtonType ButtonType;

	KeyData() {}

	KeyData(int _id, EKeyState _keyState, EButtonType _buttonType)
	{
		ID = _id;
		KeyState = _keyState;
		ButtonType = _buttonType;
	}
};

class Input
{
public:
	static void Init(class GLFWwindow* _window);
	static void PreProcess();
	static void PostProcess();

	static bool IsKeyDown(EKeyType _key);
	static bool IsKeyPressed(EKeyType _key);
	static bool IsKeyReleased(EKeyType _key);

	static void SetMousePosition(glm::vec2 _pos);
	static glm::vec2 GetMousePosition();
	static glm::vec2 GetMouseDelta();

	static glm::vec2 GetMouseScrollDelta();

	static void SetCursorVisibility(bool _visibility);

private:
	static int ConvertKey(EKeyType _key);
	static void UpdateKey(EKeyType _key);

	static void MouseScrollCallback(class GLFWwindow* _window, double _xoffset, double _yoffset);
	static glm::vec2 GetGLMousePos();
	
	static std::map<EKeyType, KeyData> m_keyData;
	static glm::vec2 m_mouseCurrent;
	static glm::vec2 m_mouseLast;
	static glm::vec2 m_mouseScrollDelta;
	static class GLFWwindow* m_window;
	static bool m_cursorVisibility;
};

