#include "ext_imgui.hpp"

#include <imgui.h>

#include "glfw_context.hpp"
#include "vulkan_context.hpp"
#include "imgui_context.hpp"

namespace ExtImGui
{
	static bool g_hasQuit = false;

	void Init()
	{
		GLFW::Init();
		Vulkan::Init();
		ImGuiContext::Init();
	}

	void Shutdown()
	{
		if (g_hasQuit)
			return;

		Vulkan::Shutdown();
		GLFW::Shutdown();
	}

	bool Update()
	{
		if (g_hasQuit)
			return false;

		if (!GLFW::Update())
		{
			Shutdown();
			g_hasQuit = true;

			return false;
		}

		if(Vulkan::CanDraw())
		{
			Vulkan::NewFrame();
			ImGuiContext::Update();
			Vulkan::Update();
		}

		return true;
	}

	IObject* RegisterObject(std::unique_ptr<IObject>&& object)
	{
		return ImGuiContext::RegisterObject(std::move(object));
	}
}