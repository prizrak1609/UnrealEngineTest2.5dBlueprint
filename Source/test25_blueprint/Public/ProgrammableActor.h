// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProgrammableActor.generated.h"

UCLASS()
class TEST25_BLUEPRINT_API AProgrammableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProgrammableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnBullet(int ImpulseStrength);

	void Teleport(int Length);
	void Forward(int Length);
	void Up(int Length);

	UPROPERTY(EditDefaultsOnly, Category = "ActorSpawning")
	TSubclassOf<AActor> bulletActor;

private:
	FTimerHandle timerHandle;
};
