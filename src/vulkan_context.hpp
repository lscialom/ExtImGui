#pragma once

namespace ExtImGui
{
	namespace Vulkan
	{
		void InitFont();

		void Init();
		void Shutdown();

		void Update();

		void NewFrame();
		void Resize(int w, int h);

		bool CanDraw();
	};
}