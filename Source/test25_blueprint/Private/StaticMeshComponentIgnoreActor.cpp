// Fill out your copyright notice in the Description page of Project Settings.


#include "StaticMeshComponentIgnoreActor.h"

bool UStaticMeshComponentIgnoreActor::MoveComponentImpl(const FVector& Delta, const FQuat& NewRotation, bool bSweep, FHitResult* OutHit, EMoveComponentFlags MoveFlags, ETeleportType Teleport)
{
	bool result = Super::MoveComponentImpl(Delta, NewRotation, bSweep, OutHit, MoveFlags, Teleport);
	clearHitIfNeeded(OutHit);
	return result;
}

bool UStaticMeshComponentIgnoreActor::SweepComponent(FHitResult& OutHit, const FVector Start, const FVector End, const FQuat& ShapeWorldRotation, const FCollisionShape& CollisionShape, bool bTraceComplex)
{
	bool result = Super::SweepComponent(OutHit, Start, End, ShapeWorldRotation, CollisionShape, bTraceComplex);
	clearHitIfNeeded(&OutHit);
	return result;
}

void UStaticMeshComponentIgnoreActor::clearHitIfNeeded(FHitResult* OutHit)
{
	if (OutHit != nullptr && OutHit->bBlockingHit && OutHit->GetActor()->StaticClass()->IsChildOf(ignoreActor))
	{
		OutHit->bBlockingHit = false;
	}
}
