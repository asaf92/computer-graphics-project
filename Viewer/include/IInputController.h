#pragma once
#include "IMoving.h"
#include "IDirectional.h"

static constexpr int MOUSE_BUTTONS_NUMBER = 3;
static constexpr int MOUSE_LEFT  = 0;
static constexpr int MOUSE_RIGHT = 1;
static constexpr int MOUSE_WHEEL = 2;

class IInputController
{
public:
	virtual void KeyPress(char input, bool control, bool shift, bool alt, bool caps) = 0;
	virtual void KeyDown(int mouseButton, float duration) = 0;
	virtual void KeyRelease(int mouseButton) = 0;
	virtual void MouseMove(float deltaX, float deltaY) = 0;
	virtual void MouseClick(int mouseButton) = 0;
	virtual void MouseDoubleClick(int mouseButton) = 0;
	virtual void MouseDown(int mouseButton, float mouseDuration) = 0;
	virtual void MouseRelease(int mouseButton) = 0;
	virtual void MouseWheel(float wheel) = 0;
};