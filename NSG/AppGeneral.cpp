/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://nsg-library.googlecode.com/

Copyright (c) 2014-2015 Néstor Silveira Gorski

-------------------------------------------------------------------------------
This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#if !defined(NACL) && !defined(ANDROID) && !defined(IOS)
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "App.h"
#include "Tick.h"
#include "Check.h"
#include <memory>
#include <assert.h>

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
			s_pApp->OnMouseMove((float)(-1 + 2 * x/s_width),(float)(1 + -2*y/s_height));
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
				s_pApp->OnMouseDown((float)(-1 + 2 * x/s_width), (float)(1 + -2*y/s_height));
			}
		}
		else
			s_pApp->OnMouseUp();
	}

	void WindowKey(GLFWwindow* window, int key, int scancode, int action, int modifier)
	{
		s_pApp->OnKey(key, action, modifier);
	}

	void WindowChar(GLFWwindow* window, unsigned int character)
	{
		s_pApp->OnChar(character);
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

	bool CreateModule(App* pApp)
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

        CHECK_GL_STATUS(__FILE__, __LINE__);

       	WindowSizeCB(window, WIDTH, HEIGHT);

        while (!glfwWindowShouldClose(window) && !s_pApp->ShallExit())
		{
            s_pApp->PerformTick();
			s_pApp->RenderFrame();
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		s_pApp = nullptr;

        glfwTerminate();
		
		return true;
	}
}
#endif