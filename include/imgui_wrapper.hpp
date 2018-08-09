#pragma once

#ifdef IMGUI_WRAPPER_DLL
	#ifdef IMGUI_WRAPPER_DLL_EXPORTS
		#define IMGUI_WRAPPER_EXPORTS __declspec(dllexport)
	#else
		#define IMGUI_WRAPPER_EXPORTS __declspec(dllimport)
	#endif
#else
	#define IMGUI_WRAPPER_EXPORTS extern
#endif

namespace ImGuiWrapper
{
	IMGUI_WRAPPER_EXPORTS void Init();
	IMGUI_WRAPPER_EXPORTS void Shutdown();

	IMGUI_WRAPPER_EXPORTS bool Update();
}