// Copyright 2015 Vagen Ayrapetyan

#include "Rage.h"

#include "Vehicle/RageBaseCar.h"
#include "Wheels/RageWheelFront.h"
#include "Wheels/RageWheelRear.h"
#include "Weapons/Weapon.h"


#include "Components/SkeletalMeshComponent.h"
#include "Sound/SoundCue.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Vehicles/WheeledVehicleMovementComponent4W.h"
#include "Engine/SkeletalMesh.h"

#include "UnrealNetwork.h"


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



	// Setup friction materials
	static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> SlipperyMat(TEXT("/Game/VehicleAdv/PhysicsMaterials/Slippery.Slippery"));
	SlipperyMaterial = SlipperyMat.Object;

	static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> NonSlipperyMat(TEXT("/Game/VehicleAdv/PhysicsMaterials/NonSlippery.NonSlippery"));
	NonSlipperyMaterial = NonSlipperyMat.Object;

}

// Begin Play
void ARageBaseCar::BeginPlay()
{
	Super::BeginPlay();
	bDead = false;

	// Start an engine sound playing
	EngineSoundComponent->Play();


	FTimerHandle MyHandle;
	GetWorldTimerManager().SetTimer(MyHandle, this, &ARageBaseCar::EquipDefaultWeapons, 2, false);
}

// The Tick
void ARageBaseCar::Tick(float Delta)
{

	Super::Tick(Delta);
	
	// Update phsyics material
	UpdatePhysicsMaterial();


	// Pass the engine RPM to the sound component
	float RPMToAudioScale = 2500.0f / GetVehicleMovement()->GetEngineMaxRotationSpeed();
	EngineSoundComponent->SetFloatParameter(EngineAudioRPM, GetVehicleMovement()->GetEngineRotationSpeed()*RPMToAudioScale);
}


// Add Health to player
void ARageBaseCar::AddHealth(float AddValue)
{
	if (Health + AddValue <= MaxHealth)
		Health += AddValue;
	else Health = MaxHealth;
}

// Get Health Percent
float ARageBaseCar::GetHealthPercent()
{
	if (Health>=0 && MaxHealth>0 )
	{
		return Health / MaxHealth;
	}
	return 0;
}

// Update Physics material
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

// Take Damage
float ARageBaseCar::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	// 
	if (bDead)
		return 0;

	Health -= DamageAmount;

	// Death Event
	if (Health <= 0)
	{
		Health = 0;
		Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
		//ServerDie();
	}


	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}


// Get Move Speed
int32 ARageBaseCar::GetMoveSpeed()
{
	if (GetVehicleMovement())return FMath::Abs(GetVehicleMovement()->GetForwardSpeed()) * 0.036f;
	else return -1;
	/*
	int32 KPH_int = FMath::FloorToInt(KPH);

	// Using FText because this is display text that should be localizable
	SpeedDisplayString = FText::Format(LOCTEXT("SpeedFormat", "{0} km/h"), FText::AsNumber(KPH_int));

	//printg("Update HUD string value");
	if (bInReverseGear == true)
	{
		GearDisplayString = FText(LOCTEXT("ReverseGear", "R"));
	}
	else
	{
		int32 Gear =;
		GearDisplayString = (Gear == 0) ? LOCTEXT("N", "N") : FText::AsNumber(Gear);
	}
	*/
}

// Get Current Gear State
int32 ARageBaseCar::GetGearState()
{
	if (GetVehicleMovement())return  GetVehicleMovement()->GetCurrentGear();
	else return 0;
}


// Replication of data
void ARageBaseCar::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	DOREPLIFETIME(ARageBaseCar, bDead);
	DOREPLIFETIME(ARageBaseCar, MainWeapon);
	DOREPLIFETIME(ARageBaseCar, AltWeapon);
	DOREPLIFETIME(ARageBaseCar, UltWeapon);
}