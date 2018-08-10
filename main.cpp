#include "ext_imgui.hpp"

int main(int, char**)
{
	ExtImGui::Init();
	//ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Main loop
	while (ExtImGui::Update())
	{
		//memcpy(&wd->ClearValue.color.float32[0], &clear_color, 4 * sizeof(float));
	}

	ExtImGui::Shutdown();

	return 0;
}
