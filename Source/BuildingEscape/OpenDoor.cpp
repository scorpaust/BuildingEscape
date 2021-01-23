// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	DoorLastOpened = 0.0f;

	DoorCloseDelay = .5f;

	DoorOpenSpeed = 1.8f;

	DoorCloseSpeed = 1.4f;

	MassToOpenDoors = 50.0f;

	bOpenDoorSound = false;

	bCloseDoorSound = true;

	PressurePlate = nullptr;

	AudioComponent = nullptr;

	// MeshToRotate = nullptr;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// InitialYaw = GetOwner()->GetActorRotation().Yaw;

	CurrentYaw = InitialYaw;

	OpenAngle += InitialYaw;

	FindMeshToRotate();

	FindPressurePlate();

	FindAudioComponent();

}

void UOpenDoor::FindPressurePlate() {

	if (!PressurePlate) {

		UE_LOG(LogTemp, Error, TEXT("The %s has the open door component on it, but no presure plate set"), *GetOwner()->GetName());

	}

}

void UOpenDoor::FindAudioComponent() {

	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent) {

		UE_LOG(LogTemp, Error, TEXT("%s missing audio component."), *GetOwner()->GetName());

	}
}

bool UOpenDoor::FindMeshToRotate() {

	MeshToRotate = GetOwner()->FindComponentByClass<UStaticMeshComponent>();

	if (MeshToRotate->GetName() == TEXT("SM_Cell_Door")) {

		return true;

	}

	return false;

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

	if (!FindMeshToRotate()) return;

	FRotator DoorRotation = MeshToRotate->GetRelativeRotation();

	DoorRotation.Yaw = CurrentYaw;

	MeshToRotate->SetRelativeRotation(DoorRotation);

	bCloseDoorSound = false;

	if (!AudioComponent) return;

	if (!bOpenDoorSound) {

		AudioComponent->Play();

		bOpenDoorSound = true;

	}

}

void UOpenDoor::CloseDoor(float DeltaTime) {

	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * DoorCloseSpeed);

	if (!FindMeshToRotate()) return;

	FRotator DoorRotation = MeshToRotate->GetRelativeRotation();

	DoorRotation.Yaw = CurrentYaw;

	MeshToRotate->SetRelativeRotation(DoorRotation);

	// FRotator DoorRotation = GetOwner()->GetActorRotation();

	// DoorRotation.Yaw = CurrentYaw;

	// GetOwner()->SetActorRotation(DoorRotation);

	bOpenDoorSound = false;

	if (!AudioComponent) return;

	if (!bCloseDoorSound) {

		AudioComponent->Play();

		bCloseDoorSound = true;

	}

}

float UOpenDoor::TotalMassOfActors() const {

	float TotalMass = 0.f;

	// Find all overlapping assets

	TArray<AActor*> OverlappingActors;

	if (!PressurePlate) return TotalMass;

	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (AActor* Actor : OverlappingActors) {

		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();

	}

	// Add up their masses

	return TotalMass;

}