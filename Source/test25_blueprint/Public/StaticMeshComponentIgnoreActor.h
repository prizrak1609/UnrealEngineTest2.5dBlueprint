// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "StaticMeshComponentIgnoreActor.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=Custom, meta=(BlueprintSpawnableComponent))
class TEST25_BLUEPRINT_API UStaticMeshComponentIgnoreActor : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Behaviour")
	TSubclassOf<AActor> ignoreActor;

	virtual bool MoveComponentImpl(const FVector& Delta, const FQuat& NewRotation, bool bSweep, FHitResult* OutHit = NULL, EMoveComponentFlags MoveFlags = MOVECOMP_NoFlags, ETeleportType Teleport = ETeleportType::None) override;

	virtual bool SweepComponent(FHitResult& OutHit, const FVector Start, const FVector End, const FQuat& ShapeWorldRotation, const FCollisionShape& CollisionShape, bool bTraceComplex = false) override;

private:
	void clearHitIfNeeded(FHitResult* OutHit = nullptr);
};
