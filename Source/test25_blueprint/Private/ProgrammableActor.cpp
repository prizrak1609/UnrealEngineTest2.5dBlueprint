// Fill out your copyright notice in the Description page of Project Settings.


#include "ProgrammableActor.h"
#include "CodeParser.h"
#include "Runtime\Core\Public\Misc\OutputDeviceNull.h"

// Sets default values
AProgrammableActor::AProgrammableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProgrammableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProgrammableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProgrammableActor::SpawnBullet(int ImpulseStrength)
{
	FGraphEventRef task = FFunctionGraphTask::CreateAndDispatchWhenReady([this, ImpulseStrength]
		{
			FVector location = GetRootComponent()->GetComponentLocation();
			location.Y += GetRootComponent()->Bounds.BoxExtent.Size() + 5;
			FActorSpawnParameters params;
			params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
			AActor* bullet = GWorld->SpawnActor<AActor>(bulletActor, location, FRotator::ZeroRotator, params);
			if (bullet == nullptr)
			{
				UCodeParser::PrintError(TEXT("Cannot spawn bullet"));
				return;
			}
			UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(bullet->GetRootComponent());
			if (mesh == nullptr)
			{
				UCodeParser::PrintError(TEXT("Cannot get bullet Mesh"));
				return;
			}
			else
			{
				FVector impulse = FVector::YAxisVector;
				mesh->AddImpulse(impulse * ImpulseStrength);
			}
		}, TStatId{}, nullptr, ENamedThreads::GameThread);

	FTaskGraphInterface::Get().WaitUntilTaskCompletes(task);
}

void AProgrammableActor::Move(int Length)
{
	FOutputDeviceNull device;
	FString function = FString::Printf(TEXT("MoveEvent %d"), Length);
	CallFunctionByNameWithArguments(*function, device, nullptr, true);
}
