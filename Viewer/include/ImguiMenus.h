#pragma once
#include <imgui/imgui.h>
#include "Scene.h"
#include "ProjectionType.h"

void DrawMenus(ImGuiIO& io, Scene& scene);
void ShowTransformationMatrices(ImGuiIO& io, Scene& scene);
void ShowProjectionControls(ImGuiIO & io, Scene & scene);
void ShowOrtographicProjectionControls(Camera & activeCamera);
void ShowPerspectiveProjectionControls(Camera & activeCamera);
void DrawImguiMenus(ImGuiIO& io, Scene& scene);
void ShowModelControls(ImGuiIO& io, Scene& scene);
void ShowLightsControls(ImGuiIO& io, Scene& scene);
void xyzSliders(glm::vec4 &newVector, std::string title, float radius);
void ShowCameraControls(ImGuiIO& io, Scene& scene);
void ShowShaderControls(ImGuiIO& io, Scene& scene);
void ShowFogControls(ImGuiIO& io, Scene& scene);
const glm::vec4& GetClearColor();
void DisplayMenuBar(ImGuiIO& io, Scene& scene);
void PrintMatrix(const glm::mat4x4& matrix, const char* name);