// Fill out your copyright notice in the Description page of Project Settings.


#include "ProgrammableActor.h"
#include "CodeParser.h"
#include "Runtime\Core\Public\Misc\OutputDeviceNull.h"
#include "ProjectUtils.h"

// Sets default values
AProgrammableActor::AProgrammableActor() : currentState(State::IDLE)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AProgrammableActor::BeginPlay()
{
	Super::BeginPlay();
	UMeshComponent* meshComponent = Cast<UMeshComponent>(GetRootComponent());
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
			UMeshComponent* mesh = Cast<UMeshComponent>(bullet->GetRootComponent());
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
			UMeshComponent* component = Cast<UMeshComponent>(GetRootComponent());
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
		currentState = State::MOVING;
		FGraphEventRef task = FFunctionGraphTask::CreateAndDispatchWhenReady([Length, this] {
				UProjectUtils::Print(FString::Printf(TEXT("Forward started %d"), Length));

				FVector currentLocation = FVector::ZeroVector;

				PrepareForMove(MoveDirection::LEFT, currentLocation);

				// last point
				currentLocation.Y += Length;
				movementComponent->AddControlPointPosition(currentLocation);
				
				StartMoving();
			}, TStatId{}, nullptr, ENamedThreads::GameThread);

		FTaskGraphInterface::Get().WaitUntilTaskCompletes(task);
	}
	else
	{
		UProjectUtils::PrintError("move component forward is null");
	}
}

void AProgrammableActor::Up(int Length)
{
	if (movementComponent != nullptr)
	{
		currentState = State::MOVING;
		FGraphEventRef task = FFunctionGraphTask::CreateAndDispatchWhenReady([Length, this] {
			UProjectUtils::Print(FString::Printf(TEXT("Up started %d"), Length));

			FVector currentLocation = FVector::ZeroVector;

			PrepareForMove(MoveDirection::UP, currentLocation);

			// last point
			currentLocation.Z += Length;
			movementComponent->AddControlPointPosition(currentLocation);
			
			StartMoving();
			}, TStatId{}, nullptr, ENamedThreads::GameThread);

		FTaskGraphInterface::Get().WaitUntilTaskCompletes(task);
	}
	else
	{
		UProjectUtils::PrintError("move component up is null");
	}
}

// this fucntion is executed from blueprint so on Game thread
void AProgrammableActor::MovementStopped()
{
	UProjectUtils::Print(TEXT("Movement stopped"));
	UMeshComponent* component = Cast<UMeshComponent>(GetRootComponent());
	component->SetSimulatePhysics(true);
	component->BodyInstance.bLockZTranslation = true;
	component->BodyInstance.bLockYTranslation = true;
	component->SetConstraintMode(EDOFMode::SixDOF);
	currentState = State::IDLE;
}

void AProgrammableActor::SetUpdatedComponentIfNeeded(UMeshComponent* component)
{
	if (movementComponent->UpdatedComponent == nullptr)
	{
		movementComponent->SetUpdatedComponent(component);
	}
}

void AProgrammableActor::PrepareForMove(MoveDirection direction, FVector initialPoint)
{
	UMeshComponent* component = Cast<UMeshComponent>(GetRootComponent());
	component->SetSimulatePhysics(false);
	switch (direction)
	{
	case MoveDirection::UP: 
	case MoveDirection::DOWN:
		component->BodyInstance.bLockZTranslation = true;
		break;
	case MoveDirection::RIGHT:
	case MoveDirection::LEFT:
		component->BodyInstance.bLockYTranslation = true;
		break;
	}
	component->SetConstraintMode(EDOFMode::SixDOF);

	SetUpdatedComponentIfNeeded(component);

	movementComponent->ResetControlPoints();

	// first point
	movementComponent->AddControlPointPosition(initialPoint);
}

void AProgrammableActor::StartMoving()
{
	movementComponent->FinaliseControlPoints();
	movementComponent->RestartMovement();
}
