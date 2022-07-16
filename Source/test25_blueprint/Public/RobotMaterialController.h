// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IMaterialController.h"

/**
 * 
 */
class TEST25_BLUEPRINT_API RobotMaterialController : public IMaterialController
{
public:
	RobotMaterialController();
	~RobotMaterialController();

	virtual void SetState(IState& state) override;
};
