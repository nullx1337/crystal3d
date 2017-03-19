#include "Engine.h"

#include "graphics\opengl\GLRenderer.h"

#ifdef Cr_WINDOWS
#include "window\win32\Win32Window.h"
#include "input\xinput\XIInputManager.h"
#endif

Core::CrEngine* Core::CrEngine::s_SharedInstance = nullptr;

namespace Core
{
	CrEngine::CrEngine()
		: m_MainWindow(nullptr), m_ActiveScene(nullptr), m_GameTimer(nullptr), m_ResourceManager(nullptr),
		m_InputManager(nullptr), m_Renderer(nullptr)
	{
		CrEngine::s_SharedInstance = this;
	}

	CrEngine::~CrEngine()
	{
		delete m_MainWindow;
		delete m_ActiveScene;
		delete m_GameTimer;
		delete m_ResourceManager;
		delete m_InputManager;
		delete m_Renderer;
	}

	bool CrEngine::Initialize()
	{
#ifdef Cr_WINDOWS
		m_MainWindow = new Window::Windows32::Win32Window();
		m_InputManager = new Input::XInput::XIInputManager();
#endif

		m_GameTimer = new Core::CrGameTimer();
		m_ResourceManager = new Resources::CrResourceManager();

		Window::CrWindowContext windowContext{};
		windowContext.title = ENGINE_WINDOW_TITLE;
		windowContext.width = ENGINE_WINDOW_WIDTH;
		windowContext.height = ENGINE_WINDOW_HEIGHT;
		windowContext.fullscreen = false;

		if (!m_MainWindow->Initialize(windowContext))
		{
			CrAssert(0, "Window failed to initialize.");
			return false;
		}

		m_MainWindow->OnClose(std::bind(&CrEngine::Quit, this));

		m_Renderer = new Graphics::OpenGL::GLRenderer();

		Graphics::CrRendererContext rendererContext{};
		rendererContext.targetWindow = m_MainWindow;
		rendererContext.viewportHeight = windowContext.height;
		rendererContext.viewportWidth = windowContext.width;

		if (!m_Renderer->Initialize(rendererContext))
		{
			CrAssert(0, "Renderer failed to initialize.");
			return false;
		}

		return true;
	}

	void CrEngine::Run()
	{
		m_MainWindow->Show();
		m_IsRunning = true;

		while (m_IsRunning)
		{
			const float_t delta = m_GameTimer->GetDelta<float_t>();

			if (delta >= 1.0f / MAX_FPS)
			{
				//TODO: MOVE WIN32 STUFF
				MSG msg{};
				while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				m_InputManager->Poll();
				this->Update(delta);
				this->Render();

				m_GameTimer->Reset();
			}
		}
	}

	void CrEngine::SetScene(Scene::CrScene* a_GameScene)
	{
		CrAssert(a_GameScene != nullptr, "Scene was nullptr!");

		m_ActiveScene = a_GameScene;
		m_ActiveScene->Initialize();
	}

	void CrEngine::Quit()
	{
		CrLog("Shutting down...");
		m_IsRunning = false;
	}

	Window::IWindow * CrEngine::GetMainWindow()
	{
		return m_MainWindow;
	}

	Scene::CrScene * CrEngine::GetCurrentScene()
	{
		return m_ActiveScene;
	}

	Core::CrGameTimer * CrEngine::GetGameTimer()
	{
		return m_GameTimer;
	}

	Resources::CrResourceManager * CrEngine::GetResourceManager()
	{
		return m_ResourceManager;
	}

	Input::IInputManager * CrEngine::GetInputManager()
	{
		return m_InputManager;
	}

	Graphics::IRenderer * CrEngine::GetRenderer()
	{
		return m_Renderer;
	}

	void CrEngine::Render()
	{
		if (m_ActiveScene != nullptr)
		{
			m_Renderer->Render(m_ActiveScene);
		}
	}

	void CrEngine::Update(const float& delta) const
	{
		if (m_ActiveScene != nullptr)
		{
			m_ActiveScene->Update(delta);
		}
	}
}