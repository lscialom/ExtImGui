#include "imgui_context.hpp"

#include "glfw_context.hpp"
#include "vulkan_context.hpp"
#include "ext_imgui.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <vector>

namespace ExtImGui
{
	static std::vector<std::unique_ptr<IObject>> g_objects;

	void ImGuiContext::Init()
	{
		// Setup Dear ImGui binding
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

		// Setup GLFW binding
		ImGui_ImplGlfw_InitForVulkan(GLFWContext::GetWindow(), true);

		// Setup style
		ImGui::StyleColorsDark();

		VkContext::InitFont();
	}

	void ImGuiContext::Shutdown()
	{
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiContext::Update()
	{
		static bool show_demo_window = true;
		static bool show_another_window = false;

		// Start the Dear ImGui frame
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		for (auto& o : g_objects)
			o->Update();

		// Rendering
		ImGui::Render();
	}

	IObject* ImGuiContext::RegisterObject(std::unique_ptr<IObject>&& object)
	{
		g_objects.emplace_back(std::move(object));
		return g_objects.back().get();
	}
}