#pragma once

#ifdef EXT_IMGUI_DLL
	#ifdef EXT_IMGUI_DLL_EXPORTS
		#define EXT_IMGUI_EXPORTS __declspec(dllexport)
	#else
		#define EXT_IMGUI_EXPORTS __declspec(dllimport)
	#endif
#else
	#define EXT_IMGUI_EXPORTS extern
#endif

namespace ExtImGui
{
	EXT_IMGUI_EXPORTS void Init();
	EXT_IMGUI_EXPORTS void Shutdown();

	EXT_IMGUI_EXPORTS bool Update();
}