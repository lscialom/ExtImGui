#pragma once

struct GLFWwindow;

typedef void (*GLFWResizeCallbackfn)(GLFWwindow *, int, int);

#include <cstdint>

namespace ExtImGui {
namespace GLFW {
void Init(uint32_t resX, uint32_t resY);
void Shutdown();

bool Update();

GLFWwindow *GetWindow();
GLFWResizeCallbackfn GetResizeCallback();
}; // namespace GLFW
} // namespace ExtImGui
