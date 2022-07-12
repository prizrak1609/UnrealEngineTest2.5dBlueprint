// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime\Core\Public\Async\TaskGraphInterfaces.h"
#include "Runtime\Engine\Classes\Components\InterpToMovementComponent.h"
#include "ITask.h"
#include "ProgrammableActor.generated.h"

UCLASS()
class TEST25_BLUEPRINT_API AProgrammableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProgrammableActor();

	enum class State : UINT
	{
		MOVING,
		IDLE
	};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void SpawnBullet(int ImpulseStrength);

	void Teleport(int Length);
	void Forward(int Length);
	void Up(int Length);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MovementStopped();

	UPROPERTY(EditDefaultsOnly, Category = "ActorSpawning")
	TSubclassOf<AActor> bulletActor;

	State currentState;

private:
	UInterpToMovementComponent* movementComponent;

	void SetUpdatedComponentIfNeeded(UMeshComponent* component);
};
