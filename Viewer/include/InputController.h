#pragma once
#include <limits.h>
#include <vector>
#include "IInputController.h"
#include "SceneActions.h"

class InputController : public IInputController
{
private:
	// Dependancies
	IMoving* _activeMovingObject;
	IDirectional* _activeDirectionalObject;
	std::vector<SceneAction> _keysMap;

	void moveCommand(char input);
public:
	InputController(IMoving* activeMovingObject,std::vector<SceneAction> source);

	// Inherited via IInputController
	virtual void KeyPress(char input, bool control, bool shift, bool alt, bool caps) override;
	virtual void KeyDown(int mouseButton, float duration) override;
	virtual void KeyRelease(int mouseButton)  override;
	virtual void MouseMove(int deltaX, int deltaY) override;
	virtual void MouseClick(int mouseButton) override;
	virtual void MouseDoubleClick(int mouseButton) override;
	virtual void MouseDown(int mouseButton, float mouseDuration) override;
	virtual void MouseRelease(int mouseButton) override;
	virtual void MouseWheel(float wheel) override;
};