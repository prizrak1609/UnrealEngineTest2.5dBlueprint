// Fill out your copyright notice in the Description page of Project Settings.


#include "RobotMaterialState.h"

RobotMaterialState* RobotMaterialState::Forward = new RobotMaterialState(RobotMaterialState::MoveDirection::FORWARD);
RobotMaterialState* RobotMaterialState::Up = new RobotMaterialState(RobotMaterialState::MoveDirection::UP);
RobotMaterialState* RobotMaterialState::Bullet = new RobotMaterialState(RobotMaterialState::MoveDirection::NONE);

RobotMaterialState::RobotMaterialState(MoveDirection direction) : IState(), moveDirection(direction)
{
}

RobotMaterialState::~RobotMaterialState()
{
}
