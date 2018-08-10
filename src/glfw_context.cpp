#include "glfw_context.hpp"

#include <cstdio>          // printf, fprintf

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vulkan_context.hpp"


namespace ExtImGui
{
	static bool g_ResizeWanted = false;
	static int g_ResizeWidth = 0, g_ResizeHeight = 0;

	static GLFWwindow* s_window = nullptr;

	static void ErrorCallback(int error, const char* description)
	{
		fprintf(stderr, "Glfw Error %d: %s\n", error, description);
	}

	static void ResizeCallback(GLFWwindow*, int w, int h)
	{
		g_ResizeWanted = true;
		g_ResizeWidth = w;
		g_ResizeHeight = h;
	}

	GLFWResizeCallbackfn GLFWContext::GetResizeCallback()
	{
		return ResizeCallback;
	}

	void GLFWContext::Init()
	{
		// Setup window
		glfwSetErrorCallback(ErrorCallback);
		if (!glfwInit())
			throw("glfw could not be initialized");

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		s_window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+Vulkan example", NULL, NULL);

		// Setup Vulkan
		if (!glfwVulkanSupported())
			throw("GLFW: Vulkan Not Supported\n");
	}

	void GLFWContext::Shutdown()
	{
		glfwDestroyWindow(s_window);
		glfwTerminate();
	}

	bool GLFWContext::Update()
	{
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		glfwPollEvents();
		if (g_ResizeWanted)
		{
			VkContext::Resize(g_ResizeWidth, g_ResizeHeight);
			g_ResizeWanted = false;
		}

		return !glfwWindowShouldClose(s_window);
	}

	GLFWwindow* GLFWContext::GetWindow()
	{
		return s_window;
	}
}