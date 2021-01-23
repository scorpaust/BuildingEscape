// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

#define OUT 

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	PhysicsHandle = nullptr;

	InputComponent = nullptr;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();

	SetupInputComponent();

}

void UGrabber::SetupInputComponent() {

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent) {

		UE_LOG(LogTemp, Warning, TEXT("Input component found on %s!"), *GetOwner()->GetName());
		InputComponent->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", EInputEvent::IE_Released, this, &UGrabber::Drop);

	}
	else {

		UE_LOG(LogTemp, Warning, TEXT("Input component missing on %s!"), *GetOwner()->GetName());

	}

}

FVector UGrabber::GetPlayersReach() const {

	// Get players viewpoint
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;

	// UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), *PlayerViewpointLocation.ToString(), *PlayerViewpointRotation.ToString());

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);

	// Draw a line from player showing the reach

	return PlayerViewpointLocation + PlayerViewpointRotation.Vector() * Reach;

}

FVector UGrabber::GetPlayersWorldPosition() const {

	// Get players viewpoint
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;

	// UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), *PlayerViewpointLocation.ToString(), *PlayerViewpointRotation.ToString());

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);

	// Draw a line from player showing the reach

	return PlayerViewpointLocation;

}

void UGrabber::FindPhysicsHandle() {

	// Checking for physics handle component

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (!PhysicsHandle) {

		UE_LOG(LogTemp, Error, TEXT("No physics handle found on %s"), *(GetOwner()->GetName()));

	}

}

void UGrabber::Grab() {

	FHitResult HitResult = GetFirstPhysicsBodyInReach();

	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	AActor* ActorHit = HitResult.GetActor();

	if (ActorHit) {

		if (!PhysicsHandle) return;

		PhysicsHandle->GrabComponentAtLocation(

			ComponentToGrab,
			NAME_None,
			GetPlayersReach()

		);

	}

}

void UGrabber::Drop() {

	if (!PhysicsHandle) return;

	PhysicsHandle->ReleaseComponent();

}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) return;

	if (PhysicsHandle->GrabbedComponent) {

		PhysicsHandle->SetTargetLocation(GetPlayersReach());

	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const {

	// Raycast out to a certain distance

	FHitResult Hit;

	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(OUT Hit, GetPlayersWorldPosition(), GetPlayersReach(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);

	return Hit;
}