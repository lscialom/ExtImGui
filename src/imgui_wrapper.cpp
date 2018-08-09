#include "imgui_wrapper.hpp"

#include "glfw_context.hpp"
#include "vulkan_context.hpp"
#include "imgui_context.hpp"

namespace ImGuiWrapper
{
	static bool g_hasQuit = false;

	void Init()
	{
		GLFWContext::Init();
		VkContext::Init();
		ImGuiContext::Init();
	}

	void Shutdown()
	{
		if (g_hasQuit)
			return;

		VkContext::Shutdown();
		GLFWContext::Shutdown();
	}

	bool Update()
	{
		if (g_hasQuit)
			return false;

		if (!GLFWContext::Update())
		{
			Shutdown();
			g_hasQuit = true;

			return false;
		}

		VkContext::NewFrame();
		ImGuiContext::Update();
		VkContext::Update();

		return true;
	}
}