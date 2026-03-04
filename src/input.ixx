module;

#include <GLFW/glfw3.h>

export module gumlib:input;

import :context;

namespace gumlib::input
{
	// helper to retrieve ContextManager safely from the window
	inline ContextManager* get_context(GLFWwindow* window)
	{
		return static_cast<ContextManager*>(glfwGetWindowUserPointer(window));
	}

	// callbacks compatible with GLFW's C-style function pointers
	inline void key_callback(GLFWwindow* window, const int key, int scancode, const int action, int mods)
	{
		InputContext& ic = get_context(window)->get_input_context();

		if (0 <= key && key < max_keyboard_buttons)
		{
			if (action == GLFW_PRESS) ic.set_keyboard_key(key, GLFW_PRESS);
			else if (action == GLFW_RELEASE) ic.set_keyboard_key(key, GLFW_RELEASE);
		}
	}

	inline void mouse_callback(GLFWwindow* window, const double xpos, const double ypos)
	{
		InputContext& ic = get_context(window)->get_input_context();
		ic.update_mouse(static_cast<float>(xpos), static_cast<float>(ypos));
	}

	inline void mouse_button_callback(GLFWwindow* window, const int button, const int action, int mods)
	{
		InputContext& ic = get_context(window)->get_input_context();

		if (0 <= button && button < max_mouse_buttons)
		{
			if (action == GLFW_PRESS) ic.set_mouse_button(button, GLFW_PRESS);
			else if (action == GLFW_RELEASE) ic.set_mouse_button(button, GLFW_RELEASE);
		}
	}

	inline void framebuffer_size_callback(GLFWwindow* window, const int width, const int height)
	{
		WindowContext& wc = get_context(window)->get_window_context();

		wc.height = height;
		wc.width = width;
		glViewport(0, 0, width, height);
	};

	// inline void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {

	// }

	// sets the window user pointer to the context and registers all callbacks
	inline void init(GLFWwindow* window, ContextManager* context)
	{
		// ReSharper disable CppTabsOutsideIndent
		glfwSetWindowUserPointer		(window, context);
		glfwSetKeyCallback				(window, key_callback);
		glfwSetCursorPosCallback		(window, mouse_callback);
		// glfwSetScrollCallback        (window, scroll_callback);
		glfwSetMouseButtonCallback		(window, mouse_button_callback);
		glfwSetFramebufferSizeCallback	(window, framebuffer_size_callback);
	}
}
