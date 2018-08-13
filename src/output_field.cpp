#include "ext_imgui.hpp"

namespace ExtImGui
{
	int OutputField::Update()
	{
		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing(); // 1 separator, 1 input text
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar); // Leave room for 1 separator + 1 InputText

		if (ImGui::BeginPopupContextWindow())
			ImGui::EndPopup();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
		ImVec4 col_default_text = ImGui::GetStyleColorVec4(ImGuiCol_Text);
		for (int i = 0; i < m_items.Size; i++)
		{
			const char* item = m_items[i];
			ImVec4 col = col_default_text;
			if (strstr(item, "[error]")) col = ImColor(1.0f, 0.4f, 0.4f, 1.0f);
			else if (strncmp(item, "# ", 2) == 0) col = ImColor(1.0f, 0.78f, 0.58f, 1.0f);
			ImGui::PushStyleColor(ImGuiCol_Text, col);
			ImGui::TextUnformatted(item);
			ImGui::PopStyleColor();
		}

		if (m_scrollToBottom)
			ImGui::SetScrollHere(1.0f);

		m_scrollToBottom = false;

		ImGui::PopStyleVar();
		ImGui::EndChild();

		return 0;
	}

	void OutputField::ClearLog()
	{
		for (int i = 0; i < m_items.Size; i++)
			free(m_items[i]);

		m_items.clear();
		m_scrollToBottom = true;
	}
}