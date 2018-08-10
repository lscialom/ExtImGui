#pragma once

#include <memory>

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

		static class IObject* RegisterObject(std::unique_ptr<class IObject>&&);
	};
}