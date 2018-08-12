#include "ext_imgui.hpp"

int main(int, char**)
{
	ExtImGui::Init();

	ExtImGui::Console* c = ExtImGui::CreateConsole();
	c->AddCommand( "echo", [=](const std::vector<std::string>& args)
	{
		std::string msg;
		for (auto& s : args)
			msg += s + "\n";
		
		c->AddLog("%s ", msg.c_str());
	});

	c->AddCommand("add", [=](const std::vector<std::string>& args)
	{
		if (args.empty())
		{
			c->AddLog("Invalid number of arguments. At least one is required.");
			return;
		}

		uint64_t number = atoi(args[0].c_str());
		for (uint8_t i = 1; i < args.size(); ++i)
			number += atoi(args[i].c_str());

		c->AddLog("%llu", number);
	});

	// Main loop
	while (ExtImGui::Update());

	ExtImGui::Shutdown();

	return 0;
}
