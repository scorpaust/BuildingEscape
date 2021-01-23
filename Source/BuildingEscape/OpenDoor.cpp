// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	DoorLastOpened = 0.0f;

	DoorCloseDelay = .5f;

	DoorOpenSpeed = 0.8f;

	DoorCloseSpeed = 0.4f;

	MassToOpenDoors = 50.0f;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;

	CurrentYaw = InitialYaw;

	OpenAngle += InitialYaw;

	if (!PressurePlate) {

		UE_LOG(LogTemp, Error, TEXT("The %s has the open door component on it, but no presure plate set"), *GetOwner()->GetName());

	}

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();

}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() > MassToOpenDoors) {

		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();

	}
	else {

		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay) {

			CloseDoor(DeltaTime);

		}

	}

}

void UOpenDoor::OpenDoor(float DeltaTime) {

	CurrentYaw = FMath::Lerp(CurrentYaw, OpenAngle, DeltaTime * DoorOpenSpeed);

	FRotator DoorRotation = GetOwner()->GetActorRotation();

	DoorRotation.Yaw = CurrentYaw;

	GetOwner()->SetActorRotation(DoorRotation);

}

void UOpenDoor::CloseDoor(float DeltaTime) {

	CurrentYaw = FMath::Lerp(CurrentYaw, 0.f, DeltaTime * DoorCloseSpeed);

	FRotator DoorRotation = GetOwner()->GetActorRotation();

	DoorRotation.Yaw = CurrentYaw;

	GetOwner()->SetActorRotation(DoorRotation);

}

float UOpenDoor::TotalMassOfActors() const {

	float TotalMass = 0.f;

	// Find all overlapping assets

	TArray<AActor*> OverlappingActors;

	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (AActor* Actor : OverlappingActors) {

		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();

	}


	// Add up their masses

	return TotalMass;

}