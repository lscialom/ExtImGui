#pragma once

struct GLFWwindow;

typedef void(*GLFWResizeCallbackfn) (GLFWwindow*, int, int);

namespace ExtImGui
{
	class GLFWContext
	{
		GLFWContext() = delete;
		~GLFWContext() = delete;

	public:
		static void Init();
		static void Shutdown();

		static bool Update();

		static GLFWwindow* GetWindow();
		static GLFWResizeCallbackfn GetResizeCallback();
	};
}