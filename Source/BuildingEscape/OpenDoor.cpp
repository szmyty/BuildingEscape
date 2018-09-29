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

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetTotalMassOnPlate() > triggerMass)
	{
		if (!Owner)
		{
			return;
		}

		onOpenRequest.Broadcast();

		//lastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if (!Owner)
		{
			return;
		}

		onCloseRequest.Broadcast();
	}

	/*if (GetWorld()->GetTimeSeconds() - lastDoorOpenTime > doorCloseDelay)
	{
		//CloseDoor();
	}*/
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
