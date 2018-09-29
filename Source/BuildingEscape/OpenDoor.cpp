// Copyright Keywi 2018

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameframeWork/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find owner component of the actor player."))
	}

	if(!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't Find Pressure Plate Object."), *(GetOwner()->GetName()))
	}
}

void UOpenDoor::OpenDoor()
{
	if (!Owner)
	{
		return;
	}

	Owner->SetActorRotation(FRotator(0.0f, openAngle, 0.0f));
}

void UOpenDoor::CloseDoor()
{
	if (!Owner)
	{
		return;
	}

	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetTotalMassOnPlate() > 50.0f)
	{
		OpenDoor();
		lastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	if (GetWorld()->GetTimeSeconds() - lastDoorOpenTime > doorCloseDelay)
	{
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOnPlate()
{
	float totalMass = 0.0f;

	TArray<AActor*> overlappingActors;

	if (!PressurePlate)
	{
		return totalMass;
	}

	PressurePlate->GetOverlappingActors(
		OUT overlappingActors
	);

	for (const auto *actor : overlappingActors)
	{
		totalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return totalMass;
}
