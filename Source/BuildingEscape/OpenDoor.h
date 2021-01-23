// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "Components/AudioComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OpenDoor(float DeltaTime);

	void CloseDoor(float DeltaTime);

	void FindAudioComponent();

	void FindPressurePlate();

	float TotalMassOfActors() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	float InitialYaw;

	float CurrentYaw;

	float DoorLastOpened;

	float DoorCloseDelay;

	bool bOpenDoorSound;

	bool bCloseDoorSound;

	UStaticMeshComponent* Mesh;

	UPROPERTY()
	UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.0f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed;

	UPROPERTY(EditAnywhere)
	float MassToOpenDoors;
	
};
