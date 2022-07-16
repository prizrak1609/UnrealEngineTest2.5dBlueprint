// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IMaterialController.h"

/**
 * 
 */
class TEST25_BLUEPRINT_API RobotMaterialState : public IMaterialController::IState
{
protected:
	enum class MoveDirection
	{
		UP,
		FORWARD,
		NONE
	};

	MoveDirection moveDirection;

    RobotMaterialState(MoveDirection direction);

public:
	static RobotMaterialState* Forward;
	static RobotMaterialState* Up;
	static RobotMaterialState* Bullet;

	virtual ~RobotMaterialState();
};
