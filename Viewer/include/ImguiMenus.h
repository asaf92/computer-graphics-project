#pragma once
#include <imgui/imgui.h>
#include "Scene.h"
#include "ProjectionType.h"
#include "IMovable.h"
#include "IDirectional.h"

void DrawMenus(ImGuiIO& io, Scene& scene);
void ShowTransformationMatrices(ImGuiIO& io, Scene& scene);
void ShowProjectionControls(ImGuiIO & io, Scene & scene);
void ShowOrtographicProjectionControls(Camera & activeCamera);
void ShowPerspectiveProjectionControls(Camera & activeCamera);
void ShowModelControls(ImGuiIO& io, Scene& scene);
void rotationControls(IRotatable* rotatable, std::string title);
void rotationControls(IRotatable* rotatable, std::string title, float rotationLimit);
void directionalControls(IDirectional* directional, std::string title);
void movingControls(IMoving * moving);
void ShowLightsControls(ImGuiIO& io, Scene& scene);
void xyzSliders(glm::vec4 &newVector, std::string title, float radius);
void ShowCameraControls(ImGuiIO& io, Scene& scene);
void moveObjectControls(IMovable* movableObject, std::string title);
void ShowShaderControls(ImGuiIO& io, Scene& scene);
void ShowFogControls(ImGuiIO& io, Scene& scene);
void DisplayMenuBar(ImGuiIO& io, Scene& scene);
void PrintMatrix(const glm::mat4x4& matrix, const char* name);