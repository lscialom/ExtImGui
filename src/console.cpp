#include "ext_imgui.hpp"

#include <string>

namespace ExtImGui
{
	// Portable helpers
	static int   Stricmp(const char* str1, const char* str2)
	{
		int d; while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1)
		{
			str1++; str2++;
		} return d;
	}
	static int   Strnicmp(const char* str1, const char* str2, int n)
	{
		int d = 0; while (n > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1)
		{
			str1++; str2++; n--;
		} return d;
	}
	static void  Strtrim(char* str)
	{
		char* str_end = str + strlen(str); while (str_end > str && str_end[-1] == ' ') str_end--; *str_end = 0;
	}

	Console::Console(OutputField* out) : m_outputField{ out }
	{
		assert(m_outputField != nullptr);

		memset(m_inputBuffer, 0, sizeof(m_inputBuffer));
		m_historyPos = -1;
		//m_commands.push_back("HELP");
		//m_commands.push_back("HISTORY");
		//m_commands.push_back("CLEAR");
		//m_commands.push_back("CLASSIFY");  // "classify" is only here to provide an example of "C"+[tab] completing to "CL" and displaying matches.
		AddLog("Welcome to Dear ImGui!");
	}

	int Console::TextEditCallback(ImGuiTextEditCallbackData* data)
	{
		//AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
		switch (data->EventFlag)
		{
		case ImGuiInputTextFlags_CallbackCompletion:
		{
			// Example of TEXT COMPLETION

			// Locate beginning of current word
			const char* word_end = data->Buf + data->CursorPos;
			const char* word_start = word_end;
			while (word_start > data->Buf)
			{
				const char c = word_start[-1];
				if (c == ' ' || c == '\t' || c == ',' || c == ';')
					break;
				word_start--;
			}

			// Build a list of candidates
			ImVector<const char*> candidates;
			for (auto& command : m_commands)
				if (Strnicmp(command.name.c_str(), word_start, (int)(word_end - word_start)) == 0)
					candidates.push_back(command.name.c_str());

			if (candidates.Size == 0)
			{
				// No match
				AddLog("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
			}
			else if (candidates.Size == 1)
			{
				// Single match. Delete the beginning of the word and replace it entirely so we've got nice casing
				data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
				data->InsertChars(data->CursorPos, candidates[0]);
				data->InsertChars(data->CursorPos, " ");
			}
			else
			{
				// Multiple matches. Complete as much as we can, so inputing "C" will complete to "CL" and display "CLEAR" and "CLASSIFY"
				int match_len = (int)(word_end - word_start);
				for (;;)
				{
					int c = 0;
					bool all_candidates_matches = true;
					for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
						if (i == 0)
							c = toupper(candidates[i][match_len]);
						else if (c == 0 || c != toupper(candidates[i][match_len]))
							all_candidates_matches = false;
					if (!all_candidates_matches)
						break;
					match_len++;
				}

				if (match_len > 0)
				{
					data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
					data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
				}

				// List matches
				AddLog("Possible matches:\n");
				for (int i = 0; i < candidates.Size; i++)
					AddLog("- %s\n", candidates[i]);
			}

			break;
		}
		case ImGuiInputTextFlags_CallbackHistory:
		{
			// Example of HISTORY
			const int prev_history_pos = m_historyPos;
			if (data->EventKey == ImGuiKey_UpArrow)
			{
				if (m_historyPos == -1)
					m_historyPos = m_history.Size - 1;
				else if (m_historyPos > 0)
					m_historyPos--;
			}
			else if (data->EventKey == ImGuiKey_DownArrow)
			{
				if (m_historyPos != -1)
					if (++m_historyPos >= m_history.Size)
						m_historyPos = -1;
			}

			// A better implementation would preserve the data on the current input line along with cursor position.
			if (prev_history_pos != m_historyPos)
			{
				data->CursorPos = data->SelectionStart = data->SelectionEnd = data->BufTextLen = (int)snprintf(data->Buf, (size_t)data->BufSize, "%s", (m_historyPos >= 0) ? m_history[m_historyPos] : "");
				data->BufDirty = true;
			}
		}
		}
		return 0;
	}

	void Console::ExecCommand(std::string_view command_line)
	{
		AddLog("# %s\n", command_line.data());

		// Insert into history. First find match and delete it so it can be pushed to the back. This isn't trying to be smart or optimal.
		m_historyPos = -1;
		for (int i = m_history.Size - 1; i >= 0; i--)
			if (Stricmp(m_history[i], command_line.data()) == 0)
			{
				free(m_history[i]);
				m_history.erase(m_history.begin() + i);
				break;
			}
		m_history.push_back(Strdup(command_line.data()));

		if (Stricmp(command_line.data(), "HELP") == 0)
		{
			AddLog("Commands:");
			for (int i = 0; i < m_commands.size(); i++)
				AddLog("- %s", m_commands[i].name.c_str());

			return;
		}

		std::string command_name;
		std::vector<std::string> args;
		std::string token;
		std::size_t word_begin = 0;

		bool quoted = false;

		for (std::size_t i = 0; i < command_line.size(); i++)
		{
			switch (command_line[i])
			{
				case '\\':
					i++;
					break;

				case '"':
					quoted = !quoted;
					break;

				case ' ':
					if (!quoted)
					{
						args.push_back({ &command_line[word_begin], i - word_begin });
						word_begin = 1+i;
					}
					break;
			}
		}

		args.push_back({ &command_line[word_begin], command_line.size() - word_begin });

		command_name = args.front();
		args.erase(args.begin());

		bool escapable = false;
		for (auto& arg : args)
		{
			for (std::size_t i = 0; i < arg.size(); i++)
			{
				escapable = false;
				switch (arg[i])
				{
				case '\\':
					arg.erase(arg.begin() + i);
					escapable = true;
					break;

				case '"':
					if (!escapable)
						arg.erase(arg.begin() + i);

					break;
				}
			}
		}
		
		for(auto& command : m_commands)
			if (command.name == command_name)
			{
				command.function(args);
				return;
			}

		AddLog("Unknown command: '%s'\n", command_line.data());
	}

	int Console::Update()
	{
		ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin("Console", &m_open))
		{
			ImGui::End();
			return 0;
		}

		// As a specific feature guaranteed by the library, after calling Begin() the last Item represent the title bar. So e.g. IsItemHovered() will return true when hovering the title bar.
		// Here we create a context menu only available from the title bar.
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Close Console"))
				m_open = false;
			ImGui::EndPopup();
		}
		//bool copy_to_clipboard = ImGui::SmallButton("Copy"); ImGui::SameLine();
		if (ImGui::SmallButton("Scroll to bottom")) m_outputField->ScrollToBottom();
		//static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t = ImGui::GetTime(); AddLog("Spam %f", t); }

		ImGui::Separator();

		//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		//static ImGuiTextFilter filter;
		//filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
		//ImGui::PopStyleVar();
		//ImGui::Separator();

		m_outputField->Update();
		
		ImGui::Separator();

		//if (m_inputField->Update() == 1)
		//{
		//	char buffer[256];
		//	strcpy(buffer, m_inputField->GetBuffer());
		//	Strtrim(buffer);
		//	if (buffer[0])
		//		ExecCommand(buffer);
		//}

		auto callback = [](ImGuiTextEditCallbackData* data) -> int
		{
			Console* console = (Console*)data->UserData;
			return console->TextEditCallback(data);
		};

		// Command-line
		bool reclaim_focus = false;
		if (ImGui::InputText("Input", m_inputBuffer, IM_ARRAYSIZE(m_inputBuffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory, callback, (void*)this))
		{
			Strtrim(m_inputBuffer);
			if (m_inputBuffer[0])
				ExecCommand(m_inputBuffer);
			strcpy(m_inputBuffer, "");
			reclaim_focus = true;
		}

		// Demonstrate keeping focus on the input box
		ImGui::SetItemDefaultFocus();
		if (reclaim_focus)
			ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

		ImGui::End();

		return 0;
	}
}