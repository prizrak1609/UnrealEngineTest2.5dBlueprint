// Fill out your copyright notice in the Description page of Project Settings.


#include "ProgrammableActor.h"
#include "CodeParser.h"
#include "Runtime\Core\Public\Misc\OutputDeviceNull.h"
#include "ProjectUtils.h"

// Sets default values
AProgrammableActor::AProgrammableActor() : currentState(State::IDLE), targetLocation(FVector::ZeroVector), speed(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AProgrammableActor::BeginPlay()
{
	Super::BeginPlay();
	UStaticMeshComponent* meshComponent = Cast<UStaticMeshComponent>(GetRootComponent());
	meshComponent->BodyInstance.bLockXTranslation = true;
	meshComponent->BodyInstance.bLockYTranslation = true;
	meshComponent->BodyInstance.bLockZTranslation = true;

	meshComponent->SetConstraintMode(EDOFMode::SixDOF);

	TInlineComponentArray<UActorComponent*> actorComponents;
	GetComponents(actorComponents);

	for (UActorComponent* actorComponent : actorComponents)
	{
		movementComponent = Cast<UInterpToMovementComponent>(actorComponent);
		if (movementComponent != nullptr)
		{
			break;
		}
	}
}

// Called every frame
void AProgrammableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UCodeParser::Print(FString::Printf(TEXT("component y %f, movement points %d"), GetRootComponent()->GetComponentLocation().Y, movementComponent->PhysicsLockedAxisSettingChanged()));
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
				UProjectUtils::PrintError(TEXT("Cannot spawn bullet"));
				return;
			}
			UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(bullet->GetRootComponent());
			if (mesh == nullptr)
			{
				UProjectUtils::PrintError(TEXT("Cannot get bullet Mesh"));
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
			component->BodyInstance.bLockYTranslation = false;
			component->SetConstraintMode(EDOFMode::SixDOF);

			FOutputDeviceNull device;
			FString function = FString::Printf(TEXT("TeleportEvent %d"), Length);
			CallFunctionByNameWithArguments(*function, device, nullptr, true);

			component->BodyInstance.bLockYTranslation = true;
			component->SetConstraintMode(EDOFMode::SixDOF);
		}, TStatId{}, nullptr, ENamedThreads::GameThread);

	FTaskGraphInterface::Get().WaitUntilTaskCompletes(task);
}

void AProgrammableActor::Forward(int Length)
{
	if (movementComponent != nullptr)
	{
		UProjectUtils::Print(FString::Printf(TEXT("Forward started %d"), Length));

		UStaticMeshComponent* component = Cast<UStaticMeshComponent>(GetRootComponent());
		component->SetSimulatePhysics(false);
		component->BodyInstance.bLockYTranslation = false;
		component->SetConstraintMode(EDOFMode::SixDOF);

		movementComponent->ResetControlPoints();
		FVector currentLocation = FVector::YAxisVector;
		// first point
		movementComponent->AddControlPointPosition(currentLocation);

		// last point
		currentLocation.Y += Length;
		movementComponent->AddControlPointPosition(currentLocation);
		movementComponent->FinaliseControlPoints();
		movementComponent->UpdateTickRegistration();

		UProjectUtils::Print(FString::Printf(TEXT("Updated component is null %d"), movementComponent->UpdatedComponent == nullptr));
	}
	else
	{
		UProjectUtils::PrintError("move component is null");
	}
}

void AProgrammableActor::Up(int Length)
{
	FGraphEventRef task = FFunctionGraphTask::CreateAndDispatchWhenReady([Length, this]
		{
			UStaticMeshComponent* component = Cast<UStaticMeshComponent>(GetRootComponent());
			component->BodyInstance.bLockZTranslation = false;
			component->SetConstraintMode(EDOFMode::SixDOF);

			UProjectUtils::Print(FString::Printf(TEXT("Up event = %d"), Length));
			FVector location = FVector::ZAxisVector * Length;
			GetRootComponent()->MoveComponent(location, FRotator::ZeroRotator, false);

			component->BodyInstance.bLockZTranslation = true;
			component->SetConstraintMode(EDOFMode::SixDOF);
		}, TStatId{}, nullptr, ENamedThreads::GameThread);

	FTaskGraphInterface::Get().WaitUntilTaskCompletes(task);
}

void AProgrammableActor::MovementStopped()
{
	UProjectUtils::Print(TEXT("Movement stopped"));
	UStaticMeshComponent* component = Cast<UStaticMeshComponent>(GetRootComponent());
	component->SetSimulatePhysics(true);
	component->BodyInstance.bLockYTranslation = true;
	component->SetConstraintMode(EDOFMode::SixDOF);
}
