// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectUtils.h"

void UProjectUtils::PrintError(FString text)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, text);
}

void UProjectUtils::Print(FString text)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, text);
}