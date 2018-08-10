#include "ext_imgui.hpp"

int main(int, char**)
{
	ExtImGui::Init();
	//ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ExtImGui::Console* c = ExtImGui::CreateConsole<ExtImGui::OutputField>();

	c->AddCommand( "echo", [=](const std::vector<std::string>& args)
	{
		std::string msg;
		for (auto& s : args)
			msg += s + "\n";
		
		c->AddLog("%s ", msg.c_str());
	});

	// Main loop
	while (ExtImGui::Update())
	{
		//memcpy(&wd->ClearValue.color.float32[0], &clear_color, 4 * sizeof(float));
	}

	ExtImGui::Shutdown();

	return 0;
}
