#pragma once
#define _USE_MATH_DEFINES

#include "ImguiMenus.h"
#include "MeshModel.h"
#include "Camera.h"
#include "Utils.h"
#include <cmath>
#include <memory>
#include <stdio.h>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <nfd.h>
#include <random>

bool showDemoWindow = false;
bool showAnotherWindow = false;

// Statics
static float worldRadius = -10.0f;

// My Menu Bools
static bool showWorldTransform =          false;
static bool showViewMatrix =              false;
static bool showProjectionMatrix =        false;
static bool showModelControls =           false;
static bool showCameraControls =          false;
static bool showProjectionControls =      false;

glm::vec4 clearColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);

const glm::vec4& GetClearColor()
{
	return clearColor;
}

void DrawMenus(ImGuiIO& io, Scene& scene)
{
	ImGui::ShowDemoWindow();
	DisplayMenuBar(io,scene);

	ImGui::Begin("Main Menu");
	
	if (ImGui::CollapsingHeader("Transformation Matrices"))
	{
		ImGui::Checkbox("World Transforamtion Matrix", &showWorldTransform); ImGui::SameLine(300.0f);
		ImGui::Checkbox("View Matrix", &showViewMatrix); ImGui::SameLine(300.0f);
		ImGui::Checkbox("Projection Matrix", &showProjectionMatrix);
	}

	if (ImGui::CollapsingHeader("Model Controls"))
	{
		ShowModelControls(io,scene);
	}

	if (ImGui::CollapsingHeader("Camera Controls"))
	{
		ShowCameraControls(io, scene);
	}

	if (ImGui::CollapsingHeader("Projection Controls"))
	{
		ShowProjectionControls(io, scene);
	}

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

	if (showWorldTransform) {}
	if (showViewMatrix) {}
	if (showProjectionMatrix) {}
	if (showModelControls)					 ShowModelControls(io,scene);
	if (showCameraControls)                  ShowCameraControls(io, scene);
}

void ShowProjectionControls(ImGuiIO& io,Scene& scene)
{
	if (scene.GetCameraCount() == 0)
	{
		ImGui::Text("No cameras available");
		return;
	}

	auto& activeCamera = scene.GetActiveCamera();
	float fov = activeCamera.GetFoV();
	float aspect = activeCamera.GetAspectRatio();
	float zNear = activeCamera.GetNear();
	float zFar = activeCamera.GetFar();

	ImGui::Text("Projection");
	ImGui::Text("Fov");
	ImGui::SliderFloat("Fov", &fov,0, 180);

	ImGui::Text("Near");
	ImGui::SliderFloat("Near", &zNear, 0,10);

	ImGui::Text("Far");
	ImGui::SliderFloat("Far", &zFar, 0, 10);
	
	activeCamera.SetPerspectiveProjection(fov, aspect, zNear, zFar);
}

void ShowCameraControls(ImGuiIO& io, Scene& scene)
{
	if (scene.GetCameraCount() == 0)
	{
		ImGui::Text("No cameras available");
		return;
	}

	auto& activeCamera = scene.GetActiveCamera();
	auto newCameraParameters = activeCamera.GetCameraParameters();

	ImGui::Text("Look At");
	ImGui::Text("Eye:");
	ImGui::SliderFloat("Eye X", &newCameraParameters[0][0], -worldRadius, worldRadius);
	ImGui::SliderFloat("Eye Y", &newCameraParameters[0][1], -worldRadius, worldRadius);
	ImGui::SliderFloat("Eye Z", &newCameraParameters[0][2], -worldRadius, worldRadius);

	ImGui::Text("At:");
	ImGui::SliderFloat("At X", &newCameraParameters[1][0], -worldRadius, worldRadius);
	ImGui::SliderFloat("At Y", &newCameraParameters[1][1], -worldRadius, worldRadius);
	ImGui::SliderFloat("At Z", &newCameraParameters[1][2], -worldRadius, worldRadius);

	ImGui::Text("Up:");
	ImGui::SliderFloat("Up X", &newCameraParameters[2][0], -worldRadius, worldRadius);
	ImGui::SliderFloat("Up Y", &newCameraParameters[2][1], -worldRadius, worldRadius);
	ImGui::SliderFloat("Up Z", &newCameraParameters[2][2], -worldRadius, worldRadius);

	ImGui::Text("Camera Poistion:");
	ImGui::Text("X:%.2f Y:%.2f Z:%.2f", newCameraParameters[0][0],newCameraParameters[0][1],newCameraParameters[0][2]);

	activeCamera.SetCameraLookAt(newCameraParameters[0], newCameraParameters[1], newCameraParameters[2]);
}

void ShowModelControls(ImGuiIO& io, Scene& scene)
{

	static float scalingSizesLimit = 5.0f;

	if (scene.GetModelCount() == 0)
	{
		ImGui::Text("No models loaded");
		return;
	}

	auto& activeModel = scene.GetActiveModel();
	auto& activeModelTranslationVector = activeModel->GetTranslationVector();
	auto& activeModelScalingSizes = activeModel->GetScalingVector();

	glm::vec3 newTranslationVector;
	newTranslationVector.x = activeModelTranslationVector.x;
	newTranslationVector.y = activeModelTranslationVector.y;
	newTranslationVector.z = activeModelTranslationVector.z;

	glm::vec3 newScalingSizes;
	newScalingSizes.x = activeModelScalingSizes.x;
	newScalingSizes.y = activeModelScalingSizes.y;
	newScalingSizes.z = activeModelScalingSizes.z;

	float newAngle = activeModel->GetRotationAngle();

	ImGui::Text("Translation");
	ImGui::SliderFloat("X Translation",&newTranslationVector.x,-worldRadius,worldRadius);
	ImGui::SliderFloat("Y Translation",&newTranslationVector.y,-worldRadius,worldRadius);
	ImGui::SliderFloat("Z Translation",&newTranslationVector.z,-worldRadius,worldRadius);

	ImGui::Text("Scaling");
	ImGui::SliderFloat("X Scale", &newScalingSizes.x, 0, scalingSizesLimit);
	ImGui::SliderFloat("Y Scale", &newScalingSizes.y, 0, scalingSizesLimit);
	ImGui::SliderFloat("Z Scale", &newScalingSizes.z, 0, scalingSizesLimit);

	ImGui::Text("Rotation");
	ImGui::SliderFloat("Y Rotation", &newAngle, -180.0f, 180.0f);

	ImGui::Text("x: %.2f y: %.2f z: %.2f", activeModelTranslationVector.x, activeModelTranslationVector.y, activeModelTranslationVector.z);

	activeModel->SetTranslation(newTranslationVector);
	activeModel->SetScaling(newScalingSizes);
	activeModel->SetRotationAngle(newAngle);
}

void DisplayMenuBar(ImGuiIO& io, Scene& scene)
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoFocusOnAppearing;
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Load Model...", "CTRL+O"))
			{
				nfdchar_t *outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("obj;png,jpg", NULL, &outPath);
				if (result == NFD_OKAY) {
					scene.AddModel(std::make_shared<MeshModel>(Utils::LoadMeshModel(outPath)));
					free(outPath);
				}
				else if (result == NFD_CANCEL) {
				}
				else {
				}

			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void DrawImguiMenus(ImGuiIO& io, Scene& scene)
{
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (showDemoWindow)
	{
		ImGui::ShowDemoWindow(&showDemoWindow);
	}

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &showDemoWindow);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &showAnotherWindow);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clearColor); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (showAnotherWindow)
	{
		ImGui::Begin("Another Window", &showAnotherWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
		{
			showAnotherWindow = false;
		}
		ImGui::End();
	}

	// 4. Demonstrate creating a fullscreen menu bar and populating it.
	{
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoFocusOnAppearing;
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Load Model...", "CTRL+O"))
				{
					nfdchar_t *outPath = NULL;
					nfdresult_t result = NFD_OpenDialog("obj;png,jpg", NULL, &outPath);
					if (result == NFD_OKAY) {
						scene.AddModel(std::make_shared<MeshModel>(Utils::LoadMeshModel(outPath)));
						free(outPath);
					}
					else if (result == NFD_CANCEL) {
					}
					else {
					}

				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}
}