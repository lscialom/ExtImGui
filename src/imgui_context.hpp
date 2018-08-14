#pragma once

#include <memory>

//Prohibited
//namespace std
//{
//	template<class T> struct default_delete;
//
//	template<class T, class Deleter>
//	class unique_ptr;
//}

namespace ExtImGui
{
	class IObject;

	namespace ImGuiContext
	{
		void Init();
		void Shutdown();

		void Update();

		IObject* RegisterObject(std::unique_ptr<IObject, std::default_delete<IObject>>&&);
	};
}