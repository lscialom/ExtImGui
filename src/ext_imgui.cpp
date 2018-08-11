#include "ext_imgui.hpp"

#include "imgui.h"

#include "glfw_context.hpp"
#include "vulkan_context.hpp"
#include "imgui_context.hpp"

namespace ExtImGui
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

		if(VkContext::CanDraw())
		{
			VkContext::NewFrame();
			ImGuiContext::Update();
			VkContext::Update();
		}

		return true;
	}

	IObject* RegisterObject(std::unique_ptr<IObject>&& object)
	{
		return ImGuiContext::RegisterObject(std::move(object));
	}
}