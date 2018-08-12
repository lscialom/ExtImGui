#include "ext_imgui.hpp"

#define NAME(var) #var

static ExtImGui::Console* g_console = nullptr;
static bool g_exit = false;

static void echo(const std::vector<std::string>& args)
{
	std::string msg;
	for (auto& s : args)
		msg += s + "\n";

	g_console->AddLog("%s ", msg.c_str());
}

static void add(const std::vector<std::string>& args)
{
	if (args.empty())
	{
		g_console->AddLog("Invalid number of arguments. At least one is required.");
		return;
	}

	uint64_t number = strtoull(args[0].c_str(), nullptr, 10);
	for (uint8_t i = 1; i < args.size(); ++i)
		number += strtoull(args[i].c_str(), nullptr, 10);

	g_console->AddLog("%llu", number);
}

static void quit(const std::vector<std::string>& args)
{
	g_exit = true;
}

int main(int, char**)
{
	ExtImGui::Init();

	g_console = ExtImGui::CreateConsole();

	g_console->AddCommand(NAME(echo), echo);
	g_console->AddCommand(NAME(add), add);
	g_console->AddCommand("exit", quit);

	// Main loop
	while (!g_exit && ExtImGui::Update());

	ExtImGui::Shutdown();

	return 0;
}
