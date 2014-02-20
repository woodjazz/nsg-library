#if !defined(NACL) && !defined(ANDROID)
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "IApp.h"
#include "Tick.h"
#include <memory>

NSG::PInternalApp s_pApp = nullptr;
static int s_width = 0;
static int s_height = 0;

namespace NSG
{
	void WindowSizeCB(GLFWwindow* window, int width, int height)
	{
		s_width = width;
		s_height = height;
		s_pApp->ViewChanged(width, height);
	}

	void WindowCursorPos(GLFWwindow* window ,double x, double y)
	{
		if(s_width > 0 && s_height > 0)
			s_pApp->OnMouseMove(-1 + 2 * x/s_width, 1 + -2*y/s_height);
	}

	void WindowScrollWheel (GLFWwindow* window, double x, double y)
	{
	}

	void WindowMouseButton(GLFWwindow* window, int button, int action, int modifier)
	{
		if(GLFW_PRESS == action)
		{
			if(s_width > 0 && s_height > 0)
			{
				double x;
                double y;
				glfwGetCursorPos(window, &x, &y); 
				s_pApp->OnMouseDown(-1 + 2 * x/s_width, 1 + -2*y/s_height);
			}
		}
		else
			s_pApp->OnMouseUp();
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

		if (glewInit() != GLEW_OK) 
        {
			fprintf(stderr, "Failed to initialize GLEW\n");
			return false;
		}

        if (!GLEW_VERSION_2_0) 
        {
            fprintf(stderr, "No support for OpenGL 2.0 found\n");
            return false;
        }

		s_pApp->Initialize(30);

		WindowSizeCB(window, WIDTH, HEIGHT);

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