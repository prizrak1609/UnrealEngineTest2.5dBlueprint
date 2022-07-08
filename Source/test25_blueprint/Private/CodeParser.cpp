// Fill out your copyright notice in the Description page of Project Settings.

#include "CodeParser.h"

#include <thread>
#include <chrono>

#include "Runtime\Core\Public\Misc\OutputDeviceNull.h"

void UCodeParser::ParseCodeCPP(FString code, AActor* actor)
{
	std::thread codeThread{ [code, actor] {
		FOutputDeviceNull device;
		FString function;

		TArray<FString> commands;
		code.ParseIntoArrayLines(commands);

		TArray<FString> chunks;
		for (FString& command : commands)
		{
			command.TrimStartAndEndInline();

			chunks.Reset();
			command.ParseIntoArray(chunks,TEXT(" "));

			for (FString& chunk : chunks)
			{
				chunk.TrimStartAndEndInline();
			}

			if (chunks[0].Compare("move", ESearchCase::IgnoreCase) == 0)
			{
				function = FString::Printf(TEXT("MoveEvent %s"), *(chunks[1]));
			}
			else
			{
				function.Empty();
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString(TEXT("Unknown command ")).Append(chunks[0]));
			}

			if (!function.IsEmpty())
			{
				actor->CallFunctionByNameWithArguments(*function, device, nullptr, true);
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
			}
		}
	} };
	codeThread.detach();
}
