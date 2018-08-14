#pragma once

struct GLFWwindow;

typedef void(*GLFWResizeCallbackfn) (GLFWwindow*, int, int);

namespace ExtImGui
{
	namespace GLFW
	{
		void Init();
		void Shutdown();

		bool Update();

		GLFWwindow* GetWindow();
		GLFWResizeCallbackfn GetResizeCallback();
	};
}