#include <ext_imgui.hpp>

#include <imgui.h>

namespace ExtImGui {
Performances *CreatePerformancesWidget() {
  auto rawp = new Performances();
  std::unique_ptr<Performances> p(rawp);

  return static_cast<Performances *>(RegisterObject(std::move(p)));
};

int Performances::Update() {
  if (!ImGui::Begin("Performances", &m_open)) {
    ImGui::End();
    return 0;
  }

  if (ImGui::BeginPopupContextItem()) {
    if (ImGui::MenuItem("Close"))
      m_open = false;
    ImGui::EndPopup();
  }

  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
              1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

  ImGui::End();

  return 0;
}
} // namespace ExtImGui
