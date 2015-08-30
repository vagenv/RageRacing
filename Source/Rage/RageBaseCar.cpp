// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Rage.h"
#include "RageBaseCar.h"
#include "RageWheelFront.h"
#include "RageWheelRear.h"
#include "Components/SkeletalMeshComponent.h"

#include "Sound/SoundCue.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Vehicles/WheeledVehicleMovementComponent4W.h"
#include "Engine/SkeletalMesh.h"
#include "Weapon.h"


const FName ARageBaseCar::EngineAudioRPM("RPM");

#define LOCTEXT_NAMESPACE "VehiclePawn"

ARageBaseCar::ARageBaseCar()
{
//	RootComponent=CreateDefaultSubobject<USceneComponent>(TEXT("TheRoot"));
	//GetMesh()->AttachParent = RootComponent;
	RootComponent = GetMesh();
	


	// Setup the audio component and allocate it a sound cue
	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCue(TEXT("/Game/VehicleAdv/Sound/Engine_Loop_Cue.Engine_Loop_Cue"));
	EngineSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineSound"));
	EngineSoundComponent->SetSound(SoundCue.Object);
	EngineSoundComponent->AttachTo(GetMesh());

	/*
	MainWeaponMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponStaticMesh"));
	AltWeaponMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponStaticMesh"));

	*/


		/*
		if (MainWeaponMeshComponent && TheActionComponent->MainWeapon && TheActionComponent->MainWeapon->TheStaticMeshComponent
			&& TheActionComponent->MainWeapon->TheStaticMeshComponent->StaticMesh)
			MainWeaponMeshComponent->SetStaticMesh(TheActionComponent->MainWeapon->TheStaticMeshComponent->StaticMesh);

		if (AltWeaponMeshComponent && TheActionComponent->AltWeapon && TheActionComponent->AltWeapon->TheStaticMeshComponent
			&& TheActionComponent->AltWeapon->TheStaticMeshComponent->StaticMesh)
			AltWeaponMeshComponent->SetStaticMesh(TheActionComponent->MainWeapon->TheStaticMeshComponent->StaticMesh);

			*/




}


void ARageBaseCar::Tick(float Delta)
{

	Super::Tick(Delta);
	// Setup the flag to say we are in reverse gear
	//bInReverseGear = GetVehicleMovement()->GetCurrentGear() < 0;
	
	// Update phsyics material
	UpdatePhysicsMaterial();


	// Pass the engine RPM to the sound component
	float RPMToAudioScale = 2500.0f / GetVehicleMovement()->GetEngineMaxRotationSpeed();
	EngineSoundComponent->SetFloatParameter(EngineAudioRPM, GetVehicleMovement()->GetEngineRotationSpeed()*RPMToAudioScale);
}

void ARageBaseCar::BeginPlay()
{
	Super::BeginPlay();

	// Start an engine sound playing
	EngineSoundComponent->Play();
}




float ARageBaseCar::GetHealthPercent()
{
	if (Health_CurrentValue>=0 && Health_MaxValue>0 )
	{
		return Health_CurrentValue / Health_MaxValue;
	}

	return 0;
}

void ARageBaseCar::UpdatePhysicsMaterial()
{
	if (GetActorUpVector().Z < 0)
	{
		if (bIsLowFriction == true)
		{
			GetMesh()->SetPhysMaterialOverride(NonSlipperyMaterial);
			bIsLowFriction = false;
		}
		else
		{
			GetMesh()->SetPhysMaterialOverride(SlipperyMaterial);
			bIsLowFriction = true;
		}
	}


}
