// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IMaterialController.h"

/**
 * 
 */
class TEST25_BLUEPRINT_API DummyMaterialController final : public IMaterialController
{
public:
	~DummyMaterialController();

	void SetState(IState& state) override;

private:
	DummyMaterialController();

	static DummyMaterialController* instance;
};
