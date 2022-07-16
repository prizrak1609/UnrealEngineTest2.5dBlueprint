// Fill out your copyright notice in the Description page of Project Settings.


#include "DummyMaterialController.h"

DummyMaterialController* DummyMaterialController::instance = new DummyMaterialController();

DummyMaterialController::DummyMaterialController() : IMaterialController()
{
}

void DummyMaterialController::SetState(IState& state)
{
}

DummyMaterialController::~DummyMaterialController()
{
}
