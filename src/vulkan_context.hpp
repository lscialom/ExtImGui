#pragma once

#include <cstdint>

namespace ExtImGui
{
	class VkContext
	{
		VkContext() = delete;
		~VkContext() = delete;

		static void SetupVulkan(const char** extensions, uint32_t extensions_count);
		static void CleanupVulkan();

	public:
		static void InitFont();

		static void Init();
		static void Shutdown();

		static void Update();

		static void NewFrame();
		static void Resize(int w, int h);
	};
}