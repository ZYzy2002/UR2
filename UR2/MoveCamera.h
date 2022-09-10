#pragma once

#include "CameraComponent.h"

class MoveCamera : public CameraComponent
{
public:
	MoveCamera(Support* pSupport);
	virtual void Tick()override;


	float moveVelocity;
	float rotateStrength;
};

