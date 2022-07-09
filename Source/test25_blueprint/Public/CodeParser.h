// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include <atomic>

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

	static void PrintError(FString text);
	static void Print(FString text);
};