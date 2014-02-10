#if !defined(NACL) && !defined(ANDROID)
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "IApp.h"
#include "Tick.h"
#include <memory>

namespace NSG
{
	struct InternalApp : public Tick
	{
		NSG::PApp pApp_;

		InternalApp(NSG::PApp pApp) : pApp_(pApp)
		{
		}

		void BeginTick()
		{
			pApp_->Start();
		}
		
		void DoTick(float delta)
		{
			pApp_->Update(delta);
		}
		
		void EndTick()
		{
			pApp_->LateUpdate();
		}

        void ViewChanged(int32_t width, int32_t height)
        {
            pApp_->ViewChanged(width, height);
        }

        void RenderFrame()
        {
            pApp_->RenderFrame();
        }
	};

	typedef std::shared_ptr<InternalApp> PInternalApp;

	PInternalApp s_pApp = nullptr;

	void WindowSizeCB(GLFWwindow* window, int width, int height)
	{
		s_pApp->ViewChanged(width, height);
	}

	void WindowCursorPos(GLFWwindow* window ,double x, double y)
	{
	}

	void WindowScrollWheel (GLFWwindow* window, double x, double y)
	{
	}

	void WindowMouseButton(GLFWwindow* window, int button, int action, int modifier)
	{
	}

	void WindowKey(GLFWwindow* window, int key, int scancode, int action, int modifier)
	{
	}

	void WindowChar(GLFWwindow* window, unsigned int character)
	{
	}

	void setupUI(GLFWwindow* window)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
		glfwSetWindowTitle(window, "Simple Example");

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		// Set GLFW event callbacks
		glfwSetWindowSizeCallback(window, WindowSizeCB);
		glfwSetMouseButtonCallback(window, WindowMouseButton);
		glfwSetCursorPosCallback(window, WindowCursorPos);
		glfwSetScrollCallback(window, WindowScrollWheel);
		glfwSetKeyCallback(window, WindowKey);
		glfwSetCharCallback(window, WindowChar);
	}

	bool CreateModule(NSG::PApp pApp)
	{
		s_pApp = PInternalApp(new InternalApp(pApp));

		GLFWwindow* window;

		if (!glfwInit())
			return false;
		
		const int WIDTH = 320;
		const int HEIGHT = 200;

		/* Create a windowed mode window and its OpenGL context */
		window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", nullptr, nullptr);
		if (!window)
		{
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(window);

		setupUI(window);

		glewExperimental = true; // Needed for core profile. Solves issue with glGenVertexArrays

		if (glewInit() != GLEW_OK) {
			fprintf(stderr, "Failed to initialize GLEW\n");
			return false;
		}

		s_pApp->Initialize(30);

		s_pApp->ViewChanged(WIDTH, HEIGHT);

		while (!glfwWindowShouldClose(window))
		{
            s_pApp->PerformTick();
			s_pApp->RenderFrame();
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glfwTerminate();
		
		return true;
	}
}
#endif