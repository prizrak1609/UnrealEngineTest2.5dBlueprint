// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementComponentIgnoreActor.h"

void UMovementComponentIgnoreActor::HandleImpact(const FHitResult& Hit, float TimeSlice, const FVector& MoveDelta)
{
	if (!Hit.GetActor()->StaticClass()->IsChildOf(ignoreActor))
	{
		Super::HandleImpact(Hit, TimeSlice, MoveDelta);
	}
}
