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
	UStaticMeshComponent* component = Cast<UStaticMeshComponent>(GetRootComponent());
	component->BodyInstance.bLockTranslation = true;
	component->BodyInstance.bLockYTranslation = true;
	component->BodyInstance.bLockZTranslation = true;

	component->SetConstraintMode(EDOFMode::SixDOF);
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

void AProgrammableActor::Teleport(int Length)
{
	FGraphEventRef task = FFunctionGraphTask::CreateAndDispatchWhenReady([Length, this]
		{
			UStaticMeshComponent* component = Cast<UStaticMeshComponent>(GetRootComponent());
			component->SetConstraintMode(EDOFMode::YZPlane);

			FOutputDeviceNull device;
			FString function = FString::Printf(TEXT("TeleportEvent %d"), Length);
			CallFunctionByNameWithArguments(*function, device, nullptr, true);

			component->SetConstraintMode(EDOFMode::SixDOF);
		}, TStatId{}, nullptr, ENamedThreads::GameThread);

	FTaskGraphInterface::Get().WaitUntilTaskCompletes(task);
}

void AProgrammableActor::Forward(int Length)
{
	FGraphEventRef task = FFunctionGraphTask::CreateAndDispatchWhenReady([this, Length]
		{
			UStaticMeshComponent* component = Cast<UStaticMeshComponent>(GetRootComponent());
			component->SetConstraintMode(EDOFMode::YZPlane);

			UCodeParser::Print(FString::Printf(TEXT("Forward event = %d"), Length));
			FVector location = FVector::YAxisVector * Length;
			GetRootComponent()->MoveComponent(location, FRotator::ZeroRotator, false);

			component->SetConstraintMode(EDOFMode::SixDOF);
		}, TStatId{}, nullptr, ENamedThreads::GameThread);

	FTaskGraphInterface::Get().WaitUntilTaskCompletes(task);
}

void AProgrammableActor::Up(int Length)
{
	FGraphEventRef task = FFunctionGraphTask::CreateAndDispatchWhenReady([Length, this]
		{
			UStaticMeshComponent* component = Cast<UStaticMeshComponent>(GetRootComponent());
			component->SetConstraintMode(EDOFMode::YZPlane);

			UCodeParser::Print(FString::Printf(TEXT("Up event = %d"), Length));
			FVector location = FVector::ZAxisVector * Length;
			GetRootComponent()->MoveComponent(location, FRotator::ZeroRotator, false);

			component->SetConstraintMode(EDOFMode::SixDOF);
		}, TStatId{}, nullptr, ENamedThreads::GameThread);

	FTaskGraphInterface::Get().WaitUntilTaskCompletes(task);
}
