#pragma once

/* Class for things that are movable and moving left/right/back/forth. */
class IMoving
{
public:
	virtual void MoveForward() = 0;
	virtual void MoveBackwards() = 0;
	virtual void MoveLeft() = 0;
	virtual void MoveRight() = 0;
};