#pragma once

namespace ExtImGui
{
	class ImGuiContext
	{
		ImGuiContext() = delete;
		~ImGuiContext() = delete;

	public:
		static void Init();
		static void Shutdown();

		static void Update();
	};
}