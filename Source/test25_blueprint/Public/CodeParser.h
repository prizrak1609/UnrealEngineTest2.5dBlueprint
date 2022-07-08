// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CodeParser.generated.h"

/**
 * 
 */
UCLASS()
class TEST25_BLUEPRINT_API UCodeParser : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void ParseCodeCPP(FString code, AActor* actor);
};
