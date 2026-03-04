module;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

export module gumlib:window;

import std;

namespace gumlib
{
	// returns a valid window pointer on success, or nullptr on failure.
	// prints specific errors to std::cerr internally.
	GLFWwindow* init_window(const int width, const int height, const char* title)
	{
		// 1. Initialize GLFW
		if (!glfwInit())
		{
			std::cerr << "Failed to initialize GLFW" << std::endl;
			return nullptr;
		}

		// 2. Configure Hints (OpenGL 4.6)
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// 3. Create Window
		// ReSharper disable CppZeroConstantCanBeReplacedWithNullptr
		GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
		if (!window)
		{
			std::cerr << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return nullptr;
		}
		glfwMakeContextCurrent(window);

		// 4. Load GLAD
		// ReSharper disable once CppCStyleCast
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cerr << "Failed to initialize GLAD" << std::endl;
			// Clean up the window before returning failure
			glfwDestroyWindow(window);
			glfwTerminate();
			return nullptr;
		}

		// 5. Setup Viewport
		glViewport(0, 0, width, height);
		glEnable(GL_DEPTH_TEST);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		return window;
	}
}
