// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class TEST25_BLUEPRINT_API IMaterialController
{
	friend class DummyMaterialController;

public:
	enum class Instance
	{
		ROBOT
	};

	class IState
	{
	public:
		virtual ~IState();

	protected: 
		IState();
	};

	template <Instance T>
	static IMaterialController* getController();

	virtual void SetState(IState& state) = 0;

	virtual ~IMaterialController();
};

template<IMaterialController::Instance T>
inline IMaterialController* IMaterialController::getController()
{
	return DummyMaterialController::instance;
}
