// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ProjectUtils.generated.h"

/**
 * 
 */
UCLASS()
class TEST25_BLUEPRINT_API UProjectUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static void PrintError(FString text);
	static void Print(FString text);
};
