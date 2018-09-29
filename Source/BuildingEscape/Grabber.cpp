// Copyright Keywi 2018

#include "Grabber.h"
#include "Engine/World.h"
#include "GameframeWork/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::Grab()
{
	auto hitResult = GetFirstPhysicsBodyInReach();
	auto componentToGrab = hitResult.GetComponent();
	auto actorHit = hitResult.GetActor();

	if (actorHit)
	{
		physicsHandle->GrabComponent(
			componentToGrab,
			NAME_None,
			componentToGrab->GetOwner()->GetActorLocation(),
			true // allow rotation.
		);
	}
}

void UGrabber::Release()
{
	physicsHandle->ReleaseComponent();
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	SetupInputComponent();
	FindPhysicsHandleComponent();
}

void UGrabber::SetupInputComponent()
{
	/// Look for attached Input Component
	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (inputComponent)
	{
		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		inputComponent->BindAction("Release", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't Find Input Component Attached to: %s. Please add one."), *(GetOwner()->GetName()))
	}
}

void UGrabber::FindPhysicsHandleComponent()
{
	/// Look for attached Physics Handle
	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (physicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't Find Physics Handle Attached to: %s. Please add one."), *(GetOwner()->GetName()))
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	FHitResult hit;

	GetWorld()->LineTraceSingleByObjectType(
		OUT hit,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	AActor *actorHit = hit.GetActor();

	if (actorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *(actorHit->GetName()))
	}
	return hit;
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (physicsHandle->GrabbedComponent)
	{
		physicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

FVector UGrabber::GetReachLineStart()
{
	FVector playerViewpointLocation;
	FRotator playerViewpointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT playerViewpointLocation,
		OUT playerViewpointRotation
	);

	return playerViewpointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
	FVector playerViewpointLocation;
	FRotator playerViewpointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT playerViewpointLocation,
		OUT playerViewpointRotation
	);

	return playerViewpointLocation + (playerViewpointRotation.Vector() * reach);
}
