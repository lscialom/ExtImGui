#include "imgui_wrapper.hpp"

int main(int, char**)
{
	ImGuiWrapper::Init();
	//ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Main loop
	while (ImGuiWrapper::Update())
	{
		//memcpy(&wd->ClearValue.color.float32[0], &clear_color, 4 * sizeof(float));
	}

	ImGuiWrapper::Shutdown();

	return 0;
}
