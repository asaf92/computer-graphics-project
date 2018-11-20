#pragma once
#include <imgui/imgui.h>
#include "Scene.h"

void DrawMenus(ImGuiIO& io, Scene& scene);
void DrawImguiMenus(ImGuiIO& io, Scene& scene);
void ShowModelControls(ImGuiIO& io, Scene& scene);
void ShowCameraControls(ImGuiIO& io, Scene& scene);
const glm::vec4& GetClearColor();
void DisplayMenuBar(ImGuiIO& io, Scene& scene);