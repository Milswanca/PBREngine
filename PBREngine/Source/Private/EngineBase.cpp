#include "EnginePCH.h"
#include "Engine.h"

EngineBase::EngineBase(ObjectInitData OI) : ObjectBase(OI)
{
	CurrentTime = 0;
	LastTime = 0;
	bWantsToClose = false;
	AssetManager::Init(this);
}


EngineBase::~EngineBase()
{
	//delete CurrentWorld;
	AssetManager::Deinit();
	glfwTerminate();
	GameInstance::DestroyInstance();
	ImGui_ImplGlfw_Shutdown();

	m_systems[ESystemType::ST_Render]->Deinit();
	m_systems[ESystemType::ST_Physics]->Deinit();

	//Destroy all objects
	for (int i = 0; i < m_allObjects.size(); ++i)
	{
		delete m_allObjects[i];
	}

	m_allObjects.clear();
}

int EngineBase::Init()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	//glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	CreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT);

	ImGui_ImplGlfw_Init(m_currentWindow, true);

	glewExperimental = true; // Needed in core profile

	if (glewInit())
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	Input::Init(m_currentWindow);

	m_renderManager = CreateObject<RenderManager>(this);
	m_systems[ESystemType::ST_Render] = m_renderManager;
	m_systems[ESystemType::ST_Render]->Init();

	m_physicsManager = CreateObject<PhysicsManager>(this);
	m_systems[ESystemType::ST_Physics] = m_physicsManager;
	m_systems[ESystemType::ST_Physics]->Init();
	
	CurrentWorld = CreateObject<World>(this);

	return 0;
}

GLFWwindow* EngineBase::CreateWindow(unsigned int width, unsigned int height)
{
	GLFWwindow* window; // (In the accompanying source code, this variable is global)
	window = glfwCreateWindow(width, height, "Tutorial 01", NULL, NULL);

	if (window)
	{
		glfwMakeContextCurrent(window); // Initialize GLEW
		m_currentWindow = window;
		GetGameInstance()->CurrentWindow = m_currentWindow;
	}

	return window;
}

void EngineBase::EngineLoop()
{
	glfwSetInputMode(m_currentWindow, GLFW_STICKY_KEYS, GL_TRUE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	double currentFrame = glfwGetTime();
	double lastFrame = currentFrame;
	double deltaTime = currentFrame - lastFrame;

	while (!glfwWindowShouldClose(m_currentWindow) && !bWantsToClose)
	{
		glfwPollEvents();
		ImGui_ImplGlfw_NewFrame();

		Input::PreProcess();

		glClearColor(0.1f, 0.1f, 0.2f, 1);

		//Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame; 
		lastFrame = currentFrame;

		//m_systems[ESystemType::ST_None]->Update(deltaTime);
		CurrentWorld->Tick(deltaTime);
		m_systems[ESystemType::ST_Physics]->Update(deltaTime);
		m_systems[ESystemType::ST_Render]->Update(deltaTime);

		DebugDraw::Render(GetGameInstance()->MainCamera->GetCameraComponent()->GetProjectionView());

		ImGui::Render();

		Input::PostProcess();

		// Swap buffers
		glfwSwapBuffers(m_currentWindow);
	}
}

void EngineBase::Shutdown()
{
	bWantsToClose = true;
}

RenderManager* EngineBase::GetRenderSystem()
{
	return m_renderManager;
}

PhysicsManager* EngineBase::GetPhysicsSystem()
{
	return m_physicsManager;
}

GLFWwindow* EngineBase::GetCurrentWindow() const
{
	return m_currentWindow;
}