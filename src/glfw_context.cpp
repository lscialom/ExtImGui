#include "glfw_context.hpp"

#include <cstdio>          // printf, fprintf

#define _GLFW_WNDCLASSNAME L"ExtImGui"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vulkan_context.hpp"


namespace ExtImGui
{
	namespace GLFW
	{
		static bool g_ResizeWanted = false;
		static int g_ResizeWidth = 0, g_ResizeHeight = 0;

		static GLFWwindow* g_window = nullptr;

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

		GLFWResizeCallbackfn GetResizeCallback()
		{
			return ResizeCallback;
		}

		void Init()
		{
			// Setup window
			glfwSetErrorCallback(ErrorCallback);
			if (!glfwInit())
				throw("glfw could not be initialized");

			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			g_window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+Vulkan example", NULL, NULL);

			// Setup Vulkan
			if (!glfwVulkanSupported())
				throw("GLFW: Vulkan Not Supported\n");
		}

		void Shutdown()
		{
			glfwDestroyWindow(g_window);
			glfwTerminate();
		}

		bool Update()
		{
			glfwPollEvents();
			if (g_ResizeWanted)
			{
				Vulkan::Resize(g_ResizeWidth, g_ResizeHeight);
				g_ResizeWanted = false;
			}

			return !glfwWindowShouldClose(g_window);
		}

		GLFWwindow* GetWindow()
		{
			return g_window;
		}
	}
}