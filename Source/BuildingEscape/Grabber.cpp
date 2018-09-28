// Copyright Keywi 2018

#include "Grabber.h"
#include "Engine/World.h"
#include "GameframeWork/PlayerController.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector playerViewpointLocation;
	FRotator playerViewpointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT playerViewpointLocation,
		OUT playerViewpointRotation
	);

	FVector lineTraceDirection = playerViewpointRotation.Vector();

	FVector lineTraceEnd = playerViewpointLocation + (lineTraceDirection * reach);

	DrawDebugLine(
		GetWorld(),
		playerViewpointLocation,
		lineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.0f,
		0,
		10.0f
	);

	/// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	FHitResult hit;

	GetWorld()->LineTraceSingleByObjectType(
		OUT hit,
		playerViewpointLocation,
		lineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

}
