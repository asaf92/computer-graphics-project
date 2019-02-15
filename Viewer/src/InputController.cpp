#include "InputController.h"
#include "SceneActions.h"
#include <algorithm>
#include <iterator>

InputController::InputController(IMoving* activeMovingObject, std::vector<SceneAction> source): 
	_activeMovingObject(activeMovingObject), 
	_keysMap(source)
{}

void InputController::KeyPress(char input, bool control, bool shift, bool alt, bool caps)
{
	switch (_keysMap[input])
	{
	case MoveForward:
	case MoveBackwards:
	case MoveLeft:
	case MoveRight:
		moveCommand(input);
	case Nothing:
	default:
		return;
	}
}

void InputController::KeyDown(int mouseButton, float duration)
{
	switch (_keysMap[mouseButton])
	{
	case MoveForward:
	case MoveBackwards:
	case MoveLeft:
	case MoveRight:
		moveCommand(mouseButton);
	case Nothing:
	default:
		return;
	}
}

void InputController::KeyRelease(int mouseButton)
{
}

void InputController::MouseMove(int deltaX, int deltaY)
{
}

void InputController::MouseClick(int mouseButton)
{
}

void InputController::MouseDoubleClick(int mouseButton)
{
}

void InputController::MouseDown(int mouseButton, float mouseDuration)
{
}

void InputController::MouseRelease(int mouseButton)
{
}

void InputController::MouseWheel(float wheel)
{
}


// Private  
void InputController::moveCommand(char input)
{
	switch (_keysMap[input])
	{
	case MoveForward:
		_activeMovingObject->MoveForward();
		break;
	case MoveBackwards:
		_activeMovingObject->MoveBackwards();
		break;
	case MoveLeft:
		_activeMovingObject->MoveLeft();
		break;
	case MoveRight:
		_activeMovingObject->MoveRight();
		break;
	default:
		return;
	}
}