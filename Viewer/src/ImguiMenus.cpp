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
static float worldRadius = 3.0f;

// My Menu Bools
static bool showModelControls =           false;
static bool showCameraControls =          false;
static bool showProjectionControls =      false;
static bool showNormals =				  false;

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
	
	if (ImGui::CollapsingHeader("General"))
	{
		ImGui::Checkbox("Show normal vectors", &showNormals);
		scene.SetShowNormals(showNormals);
	}

	if (ImGui::CollapsingHeader("Transformation Matrices"))
	{
		ShowTransformationMatrices(io,scene);
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

	if (showModelControls)					 ShowModelControls(io,scene);
	if (showCameraControls)                  ShowCameraControls(io, scene);
}

void PrintMatrix(const glm::mat4x4& matrix, const char* name)
{
	ImGui::Text(name);
	ImGui::Text("%.2f   %.2f   %.2f   %.2f ", matrix[0][0], matrix[1][0], matrix[2][0], matrix[3][0]);
	ImGui::Text("%.2f   %.2f   %.2f   %.2f ", matrix[0][1], matrix[1][1], matrix[2][1], matrix[3][1]);
	ImGui::Text("%.2f   %.2f   %.2f   %.2f ", matrix[0][2], matrix[1][2], matrix[2][2], matrix[3][2]);
	ImGui::Text("%.2f   %.2f   %.2f   %.2f ", matrix[0][3], matrix[1][3], matrix[2][3], matrix[3][3]);
}

void ShowTransformationMatrices(ImGuiIO& io, Scene& scene)
{
	if (scene.GetCameraCount() == 0) { return; }
	auto& activeCamera =     scene.GetActiveCamera();
	const auto& projectionMatrix = activeCamera.GetProjectionMatrix();
	const auto& viewMatrix =       activeCamera.GetViewMatrix();
	const auto& projectionType = activeCamera.GetProjectionType();

	bool useLibraryProjectionMatrix = activeCamera.GetUseLibraryProjectionMatrix();
	bool useLibraryViewMatrix = activeCamera.GetUseLibraryViewMatrix();

	glm::mat4 compare = glm::mat4(1);

	if (ImGui::TreeNode("Projection Matrix"))
	{
		if (projectionType  == Perspective)
		{
			PrintMatrix(projectionMatrix, "Perspective Projection Matrix");
			auto perspectiveParameters = activeCamera.GetPerspectiveProjectionParameters();
			compare = glm::perspective(perspectiveParameters.fov * 0.01745329251994329576923690768489f, perspectiveParameters.aspect, perspectiveParameters.zNear, perspectiveParameters.zFar);
			PrintMatrix(compare, "Libary Perspective Matrix");
		}
		else if (projectionType == Ortographic)
		{
			PrintMatrix(projectionMatrix, "Orthographic Projection Matrix");
			auto ortographicParameters = activeCamera.GetOrthographicProjectionParameters();
			compare = glm::ortho(ortographicParameters.left, ortographicParameters.right, ortographicParameters.bottom, ortographicParameters.top, ortographicParameters.zNear, ortographicParameters.zFar);
			PrintMatrix(compare, "Libary Ortographic Matrix");
		}
		else
		{
			ImGui::Text("No projection type specified!");
		}


		if (compare != projectionMatrix)
		{
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Libary matrix != MeshViewer matrix!");
		}
		ImGui::Checkbox("Use library matrix", &useLibraryProjectionMatrix);
		ImGui::TreePop();
	}
	if(ImGui::TreeNode("View Matrix"))
	{
		PrintMatrix(viewMatrix, "View Matrix");
		auto& activeCamera = scene.GetActiveCamera();
		LookAtParameters parameters = activeCamera.GetLookAtParameters();
		auto& eye = parameters.eye;
		auto& at = parameters.at;
		auto& up = parameters.up;
		glm::mat4 compare = glm::lookAt(eye, at, up);
		PrintMatrix(compare, "Libary lookAt Matrix");
		if (compare != viewMatrix)
		{
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Libary matrix != MeshViewer matrix!");
		}
		ImGui::Checkbox("Use library matrix", &useLibraryViewMatrix);
		ImGui::TreePop();
	}
	activeCamera.SetUseLibraryProjectionMatrix(useLibraryProjectionMatrix);
	activeCamera.SetUseLibraryViewMatrix(useLibraryViewMatrix);
	activeCamera.SetCameraLookAt(); // Update camera

	if (scene.GetModelCount() == 0){ return; }
	const auto& activeModel = scene.GetActiveModel();
	const auto& worldMatrix = activeModel->GetWorldTransformation();
	PrintMatrix(worldMatrix, "World Matrix");

}

void ShowProjectionControls(ImGuiIO& io,Scene& scene)
{
	if (scene.GetCameraCount() == 0)
	{
		ImGui::Text("No cameras available");
		return;
	}

	auto& activeCamera = scene.GetActiveCamera();
	int selection = activeCamera.GetProjectionType();
	ImGui::Text("Projection");
	ImGui::RadioButton("Ortographic Projection", &selection, Ortographic);
	ImGui::RadioButton("Perspective Projection", &selection, Perspective);
	ImGui::RadioButton("No Projection"		   , &selection, None);
	activeCamera.SelectProjectionType((ProjectionType)selection);

	switch (activeCamera.GetProjectionType()) 
	{
	case Ortographic:
		ShowOrtographicProjectionControls(activeCamera);
		break;
	case Perspective:
		ShowPerspectiveProjectionControls(activeCamera);
		break;
	case None:
	default:
		break;
	}
}

void ShowOrtographicProjectionControls(Camera& activeCamera)
{
	OrthographicProjectionParameters parameters = activeCamera.GetOrthographicProjectionParameters();
	
	ImGui::Text("Left");
	ImGui::SliderFloat("Left",   &parameters.left,   -worldRadius, worldRadius);
	ImGui::Text("Right");		 	  
	ImGui::SliderFloat("Right",  &parameters.right,  -worldRadius, worldRadius);
	ImGui::Text("Top");			 
	ImGui::SliderFloat("Top",    &parameters.top,    -worldRadius, worldRadius);
	ImGui::Text("Bottom");
	ImGui::SliderFloat("Bottom", &parameters.bottom, -worldRadius, worldRadius);
	ImGui::Text("Near");
	ImGui::SliderFloat("Near",   &parameters.zNear,  -worldRadius, worldRadius);
	ImGui::Text("Far");							    
	ImGui::SliderFloat("Far",    &parameters.zFar,   -worldRadius, worldRadius);

	activeCamera.SetOrthographicProjectionParameters(parameters);
}

void ShowPerspectiveProjectionControls(Camera & activeCamera)
{
	PerspectiveProjectionParameters parameters = activeCamera.GetPerspectiveProjectionParameters();
	float fov = activeCamera.GetFoV();
	float aspect = activeCamera.GetAspectRatio();
	float zNear = activeCamera.GetNear();
	float zFar = activeCamera.GetFar();

	ImGui::Text("Fov");
	ImGui::SliderFloat("Fov", &parameters.fov, 0, 180);

	ImGui::Text("Near");
	ImGui::SliderFloat("Near", &parameters.zNear, 0, 10);

	ImGui::Text("Far");
	ImGui::SliderFloat("Far", &parameters.zFar, 0, 10);

	activeCamera.SetPerspectiveProjectionParameters(parameters);
}

void ShowCameraControls(ImGuiIO& io, Scene& scene)
{
	if (scene.GetCameraCount() == 0)
	{
		ImGui::Text("No cameras available");
		return;
	}

	auto& activeCamera = scene.GetActiveCamera();
	//newCameraParameters = activeCamera.GetCameraParameters();
	auto newLookAtParameters = activeCamera.GetLookAtParameters();

	ImGui::Text("Look At");
	ImGui::Text("Eye:");
	ImGui::SliderFloat("Eye X", &newLookAtParameters.eye.x, -worldRadius, worldRadius);
	ImGui::SliderFloat("Eye Y", &newLookAtParameters.eye.y, -worldRadius, worldRadius);
	ImGui::SliderFloat("Eye Z", &newLookAtParameters.eye.z, -worldRadius, worldRadius);

	ImGui::Text("At:");
	ImGui::SliderFloat("At X", &newLookAtParameters.at.x, -worldRadius, worldRadius);
	ImGui::SliderFloat("At Y", &newLookAtParameters.at.y, -worldRadius, worldRadius);
	ImGui::SliderFloat("At Z", &newLookAtParameters.at.z, -worldRadius, worldRadius);

	ImGui::Text("Up:");
	ImGui::SliderFloat("Up X", &newLookAtParameters.up.x, -worldRadius, worldRadius);
	ImGui::SliderFloat("Up Y", &newLookAtParameters.up.y, -worldRadius, worldRadius);
	ImGui::SliderFloat("Up Z", &newLookAtParameters.up.z, -worldRadius, worldRadius);

	ImGui::Text("Camera Poistion:");
	ImGui::Text("X:%.2f Y:%.2f Z:%.2f", newLookAtParameters.eye.x,newLookAtParameters.eye.y,newLookAtParameters.eye.z);

	activeCamera.SetCameraLookAt(newLookAtParameters.eye, newLookAtParameters.at, newLookAtParameters.up);
}

void ShowModelControls(ImGuiIO& io, Scene& scene)
{
	if (scene.GetModelCount() == 0)
	{
		ImGui::Text("No models loaded");
		return;
	}

	int selectedModelIndex = scene.GetActiveModelIndex();
	static float scalingSizesLimit = 5.0f;
	auto& activeModel = scene.GetActiveModel();
	auto& activeModelTranslationVector = activeModel->GetTranslationVector();
	auto& activeModelScalingSizes = activeModel->GetScalingVector();
	auto& models = scene.GetModelsVector();

	static int selection_mask = (1 << 2);
	for (int i = 0; i < models.size(); i++)
	{
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selection_mask & (1 << i)) ? ImGuiTreeNodeFlags_Selected : 0);
		node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
		const char* modelName = models[i]->GetModelName().c_str();
		ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, modelName);
		if (ImGui::IsItemClicked())
			selectedModelIndex = i;
	}
	if (selectedModelIndex != -1)
	{
		// Update selection state. Process outside of tree loop to avoid visual inconsistencies during the clicking-frame.
		if (ImGui::GetIO().KeyCtrl)
			selection_mask ^= (1 << selectedModelIndex);          // CTRL+click to toggle
		else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, this commented bit preserve selection when clicking on item that is part of the selection
			selection_mask = (1 << selectedModelIndex);           // Click to single-select
	}


	glm::vec3 newTranslationVector;
	newTranslationVector.x = activeModelTranslationVector.x;
	newTranslationVector.y = activeModelTranslationVector.y;
	newTranslationVector.z = activeModelTranslationVector.z;

	glm::vec3 newScalingSizes;
	newScalingSizes.x = activeModelScalingSizes.x;
	newScalingSizes.y = activeModelScalingSizes.y;
	newScalingSizes.z = activeModelScalingSizes.z;

	glm::vec3 newAngle(0);

	ImGui::Text("Translation");
	ImGui::SliderFloat("X Translation",&newTranslationVector.x,-worldRadius,worldRadius);
	ImGui::SliderFloat("Y Translation",&newTranslationVector.y,-worldRadius,worldRadius);
	ImGui::SliderFloat("Z Translation",&newTranslationVector.z,-worldRadius,worldRadius);

	ImGui::Text("Scaling");
	ImGui::SliderFloat("X Scale", &newScalingSizes.x, 0, scalingSizesLimit);
	ImGui::SliderFloat("Y Scale", &newScalingSizes.y, 0, scalingSizesLimit);
	ImGui::SliderFloat("Z Scale", &newScalingSizes.z, 0, scalingSizesLimit);

	ImGui::Text("Rotation");
	ImGui::SliderFloat("X Rotation", &newAngle.x, -180.0f, 180.0f);
	ImGui::SliderFloat("Y Rotation", &newAngle.y, -180.0f, 180.0f);
	ImGui::SliderFloat("Z Rotation", &newAngle.z, -180.0f, 180.0f);
	

	// Leaf: The only reason we have a TreeNode at all is to allow selection of the leaf. Otherwise we can use BulletText() or TreeAdvanceToLabelPos()+Text().
	

	//if (ImGui::Button("Next Model"))
	//{
	//	if (index + 1 == models.size())
	//		scene.SetActiveModelIndex(1);
	//	else
	//		scene.SetActiveModelIndex(index + 1);
	//}

	ImGui::Text("x: %.2f y: %.2f z: %.2f", activeModelTranslationVector.x, activeModelTranslationVector.y, activeModelTranslationVector.z);

	activeModel->SetTranslation(newTranslationVector);
	activeModel->SetScaling(newScalingSizes);
	activeModel->SetRotation(newAngle);
	scene.SetActiveModelIndex(selectedModelIndex);
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