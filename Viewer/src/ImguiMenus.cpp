#pragma once
#define _USE_MATH_DEFINES

#include "ImguiMenus.h"
#include "MeshModel.h"
#include "Camera.h"
#include "Utils.h"
#include "IDirectional.h"
#include <cmath>
#include <memory>
#include <stdio.h>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <nfd.h>
#include <random>

// Globals 
bool showDemoWindow = false;
bool showAnotherWindow = false;
static float directionalSliderLimit = 2.0f;
static float rotationSliderLimit = 20.0f;
static float lightRotationSliderLimit = 1.0f;

// Shininess
static constexpr float SHININESS_MIN = 0.01f;
static constexpr float SHININESS_MAX = 500.0f;

// Toon Shading
static constexpr int TOON_SHADING_LEVELS_MIN = 1;
static constexpr int TOON_SHADING_LEVELS_MAX = 10;

// My Menu Bools
static bool showModelControls =           false;
static bool showCameraControls =          false;
static bool showProjectionControls =      false;
static bool showNormals =				  false;

// World
float worldRadius = 5.0f;

void DrawMenus(ImGuiIO& io, Scene& scene)
{
	ImGui::ShowDemoWindow();
	DisplayMenuBar(io,scene);

	worldRadius = scene.GetWorldRadius();
	ImGui::Begin("Main Menu");
	
	if (ImGui::Button("Add Banana"))
	{	
		scene.AddModel(Utils::LoadMeshModel("C:\\Users\\aagami\\Documents\\project-de-west-denya-massiv\\Data\\banana.obj"));
	}
	ImGui::SameLine();
	if (ImGui::Button("Add Crate"))
	{
		scene.AddModel(Utils::LoadMeshModel("C:\\Users\\aagami\\Documents\\project-de-west-denya-massiv\\Data\\crate.obj"));
	}
	ImGui::SameLine();
	if (ImGui::Button("Add Cow"))
	{
		scene.AddModel(Utils::LoadMeshModel("C:\\Users\\aagami\\Documents\\project-de-west-denya-massiv\\Data\\cow.obj"));
	}

	if (ImGui::CollapsingHeader("General"))
	{
		glm::vec4 clearColor = scene.GetClearColor();
		bool drawAxis = scene.GetDrawAxis();
		bool demoTriangle = scene.GetDemoTriangles();
		bool fillTriangles = scene.GetFillTriangles();
		bool showFloor = scene.GetShowFloor();
		bool toonShading = scene.GetToonShading();
		bool bumpMapping = scene.GetUseBumpMapping();
		int toonShadingLevels = scene.GetToonShadingLevels();

		ImGui::Checkbox("Show axis", &drawAxis);
		ImGui::Checkbox("Show demo triangles", &demoTriangle);
		ImGui::Checkbox("Fill triangles", &fillTriangles);
		ImGui::Checkbox("Show floor", &showFloor);
		
		// Toon Shading
		ImGui::Checkbox("Toon Shading", &toonShading); 
		ImGui::SameLine(); 
		ImGui::SliderInt("Toon Shading levels", &toonShadingLevels, TOON_SHADING_LEVELS_MIN, TOON_SHADING_LEVELS_MAX);
		
		// Bump Mapping
		ImGui::Checkbox("Bump Mapping", &bumpMapping);

		ImGui::ColorEdit3("Background color", (float*)&clearColor, ImGuiColorEditFlags_NoInputs);
		ImGui::SliderFloat("World Radius", &worldRadius, 0.1f, 10.0f);
		// Execution stats
		ImGui::Text("ImGui render execution time: %.3f", scene.GetImGuiRenderExecutionTime());
		ImGui::Text("Color buffer clearing execution time: %.3f", scene.GetColorBufferExecutionTime());
		ImGui::Text("Render execution time: %.3f", scene.GetRenderExecutionTime());

		scene.SetDrawAxis(drawAxis);
		scene.SetDemoTriangles(demoTriangle);
		scene.SetFillTriangles(fillTriangles);
		scene.SetShowFloor(showFloor);
		scene.SetClearColor(clearColor);
		scene.SetToonShading(toonShading);
		scene.SetToonShadingLevels(toonShadingLevels);
		scene.SetUseBumpMapping(bumpMapping);
	}

	if (ImGui::CollapsingHeader("Transformation Matrices"))
	{
		ShowTransformationMatrices(io,scene);
	}

	if (ImGui::CollapsingHeader("Model Controls"))
	{
		ShowModelControls(io,scene);
	}

	if (ImGui::CollapsingHeader("Lights Controls"))
	{
		ShowLightsControls(io, scene);
	}

	if (ImGui::CollapsingHeader("Camera Controls"))
	{
		ShowCameraControls(io, scene);
	}

	if (ImGui::CollapsingHeader("Projection Controls"))
	{
		ShowProjectionControls(io, scene);
	}

	if (ImGui::CollapsingHeader("Shader Controls"))
	{
		ShowShaderControls(io, scene);
	}

	if (ImGui::CollapsingHeader("Fog Controls"))
	{
		ShowFogControls(io, scene);
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
	const auto worldMatrix = activeModel->GetWorldTransformation();
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
	ImGui::SliderFloat("Far", &parameters.zFar, 0, 200);

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
	directionalControls(&activeCamera, "Camera");

	auto newLookAtParameters = activeCamera.GetLookAtParameters();
	movingControls(&activeCamera);
	moveObjectControls(&activeCamera, "Move Camera");

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
	static float scalingSizesLimit = 5.0f;

	if (scene.GetModelCount() == 0)
	{
		ImGui::Text("No models loaded");
		return;
	}

	int selectedModelIndex = scene.GetActiveModelIndex();
	MeshModel* activeModel = scene.GetActiveModel();
	auto& activeModelTranslationVector = activeModel->GetTranslationVector();
	auto& activeModelScalingSizes = activeModel->GetScale();
	auto& models = scene.GetModelsVector();
	auto color = activeModel->GetAmbientColor();

	// Uniform Material variables
	auto ambientColor = activeModel->GetAmbientColor();
	auto specularColor = activeModel->GetSpecularColor();
	auto diffuseColor = activeModel->GetDiffuseColor();
	auto shininess = activeModel->GetShininess();

	static int selection_mask = (1 << 2);
	ImGui::Text("Model Selection");
	for (unsigned int i = 0, modelsSize = models.size(); i < modelsSize; i++)
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

	moveObjectControls(activeModel, "Model");
	rotationControls(activeModel, "Model");
	if (ImGui::TreeNode("Advanced options"))
	{
		ImGui::Text("Color");
		ImGui::ColorEdit3("Ambient color", (float*)&color);

		glm::vec3 newTranslationVector;
		newTranslationVector.x = activeModelTranslationVector.x;
		newTranslationVector.y = activeModelTranslationVector.y;
		newTranslationVector.z = activeModelTranslationVector.z;

		glm::vec3 newScalingSizes;
		newScalingSizes.x = activeModelScalingSizes.x;
		newScalingSizes.y = activeModelScalingSizes.y;
		newScalingSizes.z = activeModelScalingSizes.z;

		ImGui::Text("Translation");
		ImGui::SliderFloat("X Translation",&newTranslationVector.x,-worldRadius,worldRadius);
		ImGui::SliderFloat("Y Translation",&newTranslationVector.y,-worldRadius,worldRadius);
		ImGui::SliderFloat("Z Translation",&newTranslationVector.z,-worldRadius,worldRadius);

		ImGui::Text("Scaling");
		ImGui::SliderFloat("X Scale", &newScalingSizes.x, 0, scalingSizesLimit);
		ImGui::SliderFloat("Y Scale", &newScalingSizes.y, 0, scalingSizesLimit);
		ImGui::SliderFloat("Z Scale", &newScalingSizes.z, 0, scalingSizesLimit);

		ImGui::Text("Ambient Color");
		ImGui::ColorEdit3("Ambient", (float*)&ambientColor);
		ImGui::Text("Specular Color");
		ImGui::ColorEdit3("Specular", (float*)&specularColor);
		ImGui::Text("Diffuse Color");
		ImGui::ColorEdit3("Diffuse", (float*)&diffuseColor);
		ImGui::TreePop();
		ImGui::SliderFloat("Shininess", (float*)&shininess, SHININESS_MIN, SHININESS_MAX);
		activeModel->SetTranslation(newTranslationVector);
		activeModel->Scale(newScalingSizes);
		activeModel->SetAmbientColor(ambientColor);
		activeModel->SetSpecularColor(specularColor);
		activeModel->SetDiffuseColor(diffuseColor);
		activeModel->SetShininess(shininess);
	}

	ImGui::Text("x: %.2f y: %.2f z: %.2f", activeModelTranslationVector.x, activeModelTranslationVector.y, activeModelTranslationVector.z);
	activeModel->SetAmbientColor(color);
	scene.SetActiveModelIndex(selectedModelIndex);
}

void ShowShaderControls(ImGuiIO& io, Scene& scene)
{
	return;
}

void ShowFogControls(ImGuiIO& io, Scene& scene)
{
	bool enableFog = scene.GetFogEnabled();
	ImGui::Text("Fog");
	ImGui::Checkbox("Enable fog", &enableFog);
	scene.SetFogEnabled(enableFog);

	if (!enableFog) return;
	float start = scene.GetFogStart();
	float finish = scene.GetFogFinish();
	ImGui::SliderFloat("Start", &start, 0, 10.0f);
	ImGui::SliderFloat("Finish", &finish, 0, 10.0f);

	scene.SetFogStart(start);
	scene.SetFogFinish(finish);
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
					scene.AddModel(Utils::LoadMeshModel(outPath));
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

void ShowLightsControls(ImGuiIO& io, Scene& scene)
{
	int selectedLightIndex = scene.GetActiveLightsIndex();
	glm::vec4 ambientLight = scene.GetAmbientLight();
	bool drawLightSources = scene.GetDrawLights();
	glm::vec3 lightLocation = glm::vec3(0.0f);
	glm::vec3 lightDirection = glm::vec3(0.0f);

	ImGui::ColorEdit3("Ambient light color", (float*)&ambientLight, ImGuiColorEditFlags_NoInputs);
	scene.SetAmbientLight(ambientLight);
	//int selectedLightIndex = -1;
	if (ImGui::Button("Add point light source"))
	{
		scene.AddLight(PointSource);
	}
	if (ImGui::Button("Add parallel light source"))
	{
		scene.AddLight(Parallel);
	}
	const std::vector<LightSource*> lights = scene.GetLightsVector();
	if (lights.size() == 0)
	{
		ImGui::Text("No lights in the scene");
		return;
	}
	auto activeLight = scene.GetActiveLight();
	glm::vec4 newActiveLightColor = activeLight->GetColor();

	static int selection_mask = (1 << 2);
	for (unsigned int i = 0, lightsSize = lights.size(); i < lightsSize; i++)
	{
		// Some stupid ImGui interfacing that i copy pasted just to have a single selectable node
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selection_mask & (1 << i)) ? ImGuiTreeNodeFlags_Selected : 0);
		node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
		ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, lights[i]->GetName().c_str());

		if (ImGui::IsItemClicked())
			selectedLightIndex = i;
	}
	if (selectedLightIndex != -1)
	{
		// Update selection state. Process outside of tree loop to avoid visual inconsistencies during the clicking-frame.
		if (ImGui::GetIO().KeyCtrl)
			selection_mask ^= (1 << selectedLightIndex);          // CTRL+click to toggle
		else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, this commented bit preserve selection when clicking on item that is part of the selection
			selection_mask = (1 << selectedLightIndex);           // Click to single-select
	}
	if (IMovable* movableLight = dynamic_cast<IMovable*>(activeLight))
	{
		moveObjectControls(movableLight,"Light");
	}

	if (IDirectional* directionalLight = dynamic_cast<IDirectional*>(activeLight))
	{
		auto direction = directionalLight->GetDirection();
		ImGui::Text("Direction: x:%.2f y:%.2f z:%.2f", direction.x, direction.y, direction.z);
		directionalControls(directionalLight, "Light");
	}

	ImGui::ColorEdit3("Light color", (float*)&newActiveLightColor, ImGuiColorEditFlags_NoInputs);
	ImGui::Checkbox("Draw light sources", &drawLightSources);

	scene.SetActiveLightsIndex(selectedLightIndex);
	scene.SetDrawLights(drawLightSources);
	activeLight->SetColor(newActiveLightColor);
	ImGui::Text("Number of lights: %d",scene.GetLightsCount());
	ImGui::Text("Active light number: %d", selectedLightIndex + 1);
}

#pragma region Common controls
void xyzSliders(glm::vec4 &newVector, std::string title, float radius)
{
	ImGui::Text(title.c_str());
	ImGui::SliderFloat((std::string("X ") +title).c_str(), &newVector.x, -radius, radius);
	ImGui::SliderFloat((std::string("Y ") +title).c_str(), &newVector.y, -radius, radius);
	ImGui::SliderFloat((std::string("Z ") +title).c_str(), &newVector.z, -radius, radius);
}

void moveObjectControls(IMovable* movableObject, const std::string title)
{
	static float moveRadius = 0.1f;
	glm::vec3 moveDirection = glm::vec3(0);
	ImGui::Text("Move");
	ImGui::SliderFloat(std::string("Move X##" + title).c_str(), &moveDirection.x, -moveRadius, moveRadius);
	ImGui::SliderFloat(std::string("Move Y##" + title).c_str(), &moveDirection.y, -moveRadius, moveRadius);
	ImGui::SliderFloat(std::string("Move Z##" + title).c_str(), &moveDirection.z, -moveRadius, moveRadius);
	movableObject->Move(moveDirection);
}

void rotationControls(IRotatable* rotatable, std::string title)
{
	rotationControls(rotatable, title, rotationSliderLimit);
}

void rotationControls(IRotatable* rotatable, std::string title, float rotationLimit)
{
	float limit = rotationLimit;
	glm::vec3 angle(0);
	ImGui::Text("Rotation");
	ImGui::SliderFloat(std::string("X Rotation##" + title).c_str(), &angle.x, -limit, limit);
	ImGui::SliderFloat(std::string("Y Rotation##" + title).c_str(), &angle.y, -limit, limit);
	ImGui::SliderFloat(std::string("Z Rotation##" + title).c_str(), &angle.z, -limit, limit);
	rotatable->RotateX(angle.x);
	rotatable->RotateY(angle.y);
	rotatable->RotateZ(angle.z);
}

void directionalControls(IDirectional* directional, std::string title)
{
	glm::vec2 angle(0.0f);
	const auto direction = directional->GetDirection();

	ImGui::Text("Rotation");
	ImGui::SliderFloat(std::string("Pan##" + title).c_str(), &angle.x,  -directionalSliderLimit, directionalSliderLimit);
	ImGui::SliderFloat(std::string("Tilt##" + title).c_str(), &angle.y, -directionalSliderLimit, directionalSliderLimit);
	directional->Pan(-angle.x);			// We use the negative value of the angle to get correct panning & tilting
	directional->Tilt(angle.y);
	ImGui::Text("Direction: x: %f.2,y: %f.2", direction.x, direction.y);
}

void movingControls(IMoving* moving)
{
	ImGui::Text("Move");
	if (ImGui::Button("Forward"))
	{
		moving->MoveForward();
	}
	if (ImGui::Button("Left"))
	{
		moving->MoveLeft();
	}
	ImGui::SameLine();
	if (ImGui::Button("Backwards"))
	{
		moving->MoveBackwards();
	}
	ImGui::SameLine();
	if (ImGui::Button("Right"))
	{
		moving->MoveRight();
	}
}
#pragma endregion
