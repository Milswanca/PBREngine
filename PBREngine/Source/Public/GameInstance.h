#pragma once
#include "ObjectBase.h"

class GameInstance : public ObjectBase
{
public:
	~GameInstance();

	static GameInstance* Instance()
	{
		if (!instance)
		{
			instance = new GameInstance(ObjectInitData());
		}

		return instance;
	}

	static void DestroyInstance();

	GameInstance(GameInstance const&) = delete;
	void operator=(GameInstance const&) = delete;

	class Camera* MainCamera;
	GLFWwindow* CurrentWindow;

private:
	GameInstance(ObjectInitData OI);

	static GameInstance* instance;
};

