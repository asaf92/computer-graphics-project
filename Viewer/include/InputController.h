#pragma once
#include <limits.h>
#include <vector>
#include "IInputController.h"
#include "SceneActions.h"

static float constexpr mouseSensitivity = 0.015f;

class InputController : public IInputController
{
private:
	// Dependancies
	IMoving* _activeMovingObject;
	IDirectional* _activeDirectionalObject;
	std::vector<SceneAction> _keysMap;
	std::vector<SceneAction> _mouseKeysMap;

	// Flags
	bool _directionChangeEnabled = false;

	void moveCommand(char input);
public:
	InputController(IMoving* activeMovingObject, IDirectional* activeDirectionalObject, std::vector<SceneAction> keysMap, std::vector<SceneAction> mouseKeysMap);

	// Inherited via IInputController
	virtual void KeyPress(char input, bool control, bool shift, bool alt, bool caps) override;
	virtual void KeyDown(int mouseButton, float duration) override;
	virtual void KeyRelease(int mouseButton)  override;
	virtual void MouseMove(float deltaX, float deltaY) override;
	virtual void MouseClick(int mouseButton) override;
	virtual void MouseDoubleClick(int mouseButton) override;
	virtual void MouseDown(int mouseButton, float mouseDuration) override;
	virtual void MouseRelease(int mouseButton) override;
	virtual void MouseWheel(float wheel) override;
};