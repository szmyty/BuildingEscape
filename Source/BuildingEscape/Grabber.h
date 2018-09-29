// Copyright Keywi 2018

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

private:
	float reach = 100.0f;

	UPhysicsHandleComponent *physicsHandle = nullptr;

	UInputComponent *inputComponent = nullptr;

	// Ray-cast and grab what's in reach.
	void Grab();
	// Release the object that was being held.
	void Release();

	// Find and setup the input component (assumed).
	void SetupInputComponent();

	// Find the Physics Handle attached to the player (assumed).
	void FindPhysicsHandleComponent();

	// Return hit for the object in reach.
	const FHitResult GetFirstPhysicsBodyInReach();

	// Returns current start of reach line for raycasting.
	FVector GetReachLineStart();

	// Returns current end of reach line for raycasting.
	FVector GetReachLineEnd();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	

};
