// Fill out your copyright notice in the Description page of Project Settings.

#include "CodeParser.h"
#include "ProgrammableActor.h"
#include "ProjectUtils.h"

#include <thread>
#include <chrono>

void UCodeParser::ParseCodeCPP(FString code, AActor* actor)
{
	AProgrammableActor* programmableActor = Cast<AProgrammableActor>(actor);

	if (programmableActor == nullptr)
	{
		UProjectUtils::PrintError(TEXT("Actor is not programmable"));
		return;
	}

	Async(EAsyncExecution::Thread, [code, programmableActor] {
			TArray<FString> commands;
			code.ParseIntoArrayLines(commands);

			bool error = false;

			TArray<FString> chunks;
			for (FString& command : commands)
			{
				error = false;
				command.TrimStartAndEndInline();

				chunks.Reset();
				command.ParseIntoArray(chunks, TEXT(" "));

				for (FString& chunk : chunks)
				{
					chunk.TrimStartAndEndInline();
				}

				if (chunks[0].Compare("teleport", ESearchCase::IgnoreCase) == 0)
				{
					programmableActor->Teleport(FCString::Atoi(*(chunks[1])));
				}
				else if (chunks[0].Compare("bullet", ESearchCase::IgnoreCase) == 0)
				{
					programmableActor->SpawnBullet(FCString::Atoi(*(chunks[1])));
				}
				else if (chunks[0].Compare("up", ESearchCase::IgnoreCase) == 0)
				{
					programmableActor->Up(FCString::Atoi(*(chunks[1])));
				}
				else if (chunks[0].Compare("forward", ESearchCase::IgnoreCase) == 0)
				{
					programmableActor->Forward(FCString::Atoi(*(chunks[1])));
				}
				else
				{
					UProjectUtils::PrintError(FString(TEXT("Unknown command ")).Append(chunks[0]));
					error = true;
				}

				if (!error)
				{
					while (programmableActor->currentState != AProgrammableActor::State::IDLE)
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
					}
				}
			}
		});
}
