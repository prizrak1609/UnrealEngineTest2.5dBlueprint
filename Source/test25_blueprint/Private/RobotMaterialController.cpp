// Fill out your copyright notice in the Description page of Project Settings.


#include "RobotMaterialController.h"

template<>
IMaterialController* IMaterialController::getController<IMaterialController::Instance::ROBOT>()
{
	return new RobotMaterialController();
}

RobotMaterialController::RobotMaterialController()
{
}

RobotMaterialController::~RobotMaterialController()
{
}

void RobotMaterialController::SetState(IState& state)
{
}
