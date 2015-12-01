// Copyright 2015 Vagen Ayrapetyan

#include "Rage.h"
#include "Engine.h"

#include "Vehicle/RageBaseCar.h"
#include "Wheels/RageWheelFront.h"
#include "Wheels/RageWheelRear.h"
#include "Weapons/Weapon.h"

#include "System/RagePlayerState.h"

#include "Components/SkeletalMeshComponent.h"
#include "Sound/SoundCue.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Vehicles/WheeledVehicleMovementComponent4W.h"
#include "Engine/SkeletalMesh.h"

#include "UnrealNetwork.h"

#include "Components/WidgetComponent.h"

const FName ARageBaseCar::EngineAudioRPM("RPM");

#define LOCTEXT_NAMESPACE "VehiclePawn"

ARageBaseCar::ARageBaseCar()
{
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


	CharacterStatusWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Player Status Widget"));
	CharacterStatusWidget->AttachParent = GetMesh();
	CharacterStatusWidget->bOwnerNoSee = true;

	CharacterName = "Base Character";

	FRichCurve* RichZoomCurve = CharacterStatusWidget_ZoomCurve.GetRichCurve();
	if (RichZoomCurve)
	{
		RichZoomCurve->AddKey(1000, 1);
		RichZoomCurve->AddKey(5000, 4);
	}

}

// Begin Play
void ARageBaseCar::BeginPlay()
{
	Super::BeginPlay();
	bDead = false;

	// Start an engine sound playing
	EngineSoundComponent->Play();


	TheLocalController=UGameplayStatics::GetPlayerController(GetWorld(), 0);


	FVector PositionOffset = FVector(FMath::RandRange(-200, 200), FMath::RandRange(-200, 200),0);
	SetActorLocation(GetActorLocation()+PositionOffset);

	if (CharacterStatusWidget)
		CharacterStatusWidget_DefaultOffset = CharacterStatusWidget->RelativeLocation;

	if (Role >= ROLE_Authority)
	{

		FTimerHandle MyHandle;
		GetWorldTimerManager().SetTimer(MyHandle, this, &ARageBaseCar::EquipDefaultWeapons, 1, false);


		if (bRandomizeCharacterColorOnStart)
			CharacterColor = FLinearColor(FMath::RandRange(0, 255), FMath::RandRange(0, 255), FMath::RandRange(0, 255));
	}

	// Delay Post begin play after 1 sec
	FTimerHandle PostBeginHandle;
	GetWorldTimerManager().SetTimer(PostBeginHandle, this, &ARageBaseCar::PostBeginPlay, 1, false);

}

//		Post Begin Play
void ARageBaseCar::PostBeginPlay()
{
	// Refind The Controller
	//if (!TheLocalController)
	//	TheLocalController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}


//		 The Tick
void ARageBaseCar::Tick(float Delta)
{
	Super::Tick(Delta);
	
	// Update phsyics material
	UpdatePhysicsMaterial();

	//Rotate Widget To Camera;
	if (CharacterStatusWidget && TheLocalController)
		RotateCharacterStatusWidget();


	// Pass the engine RPM to the sound component
	float RPMToAudioScale = 2500.0f / GetVehicleMovement()->GetEngineMaxRotationSpeed();
	EngineSoundComponent->SetFloatParameter(EngineAudioRPM, GetVehicleMovement()->GetEngineRotationSpeed()*RPMToAudioScale);
}


//		Rotate Widget To camera
void ARageBaseCar::RotateCharacterStatusWidget()
{
	if (!CharacterStatusWidget || !TheLocalController) return;

	if (TheLocalController->PlayerCameraManager)
	{
		// Set Widget on top of the car
		CharacterStatusWidget->SetWorldLocation(GetActorLocation() + CharacterStatusWidget_DefaultOffset);

		// Set Scale on the widget. Take distance between car and camera-> feed to curve
		CharacterStatusWidget->RelativeScale3D = FVector(CharacterStatusWidget_ZoomCurve.GetRichCurve()->Eval(FVector::Dist(GetActorLocation(), TheLocalController->PlayerCameraManager->GetCameraLocation())));
		
		// Get Camera vector
		FVector CamForwardVec = TheLocalController->PlayerCameraManager->GetActorForwardVector();
		// Flip Direction
		CamForwardVec *= -1;

		// Set Widget Rot
		CharacterStatusWidget->SetWorldRotation(CamForwardVec.Rotation());
		
	} 
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

	if (Health <= 0)
	{
		Health = 0;

		Death();

		// Add Death to Self
		if (Cast<ARagePlayerState>(PlayerState))Cast<ARagePlayerState>(PlayerState)->DeathCount++;

		// Add Kill to Enemy
		if (EventInstigator && Cast<ARageBaseCar>(EventInstigator->GetPawn()) && Cast<ARagePlayerState>(Cast<ARageBaseCar>(EventInstigator->GetPawn())->PlayerState))
			Cast<ARagePlayerState>(Cast<ARageBaseCar>(EventInstigator->GetPawn())->PlayerState)->KillCount++;

		//ServerDie();
	}


	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

// Add Health to player
void ARageBaseCar::AddHealth(float AddValue)
{
	if (Health + AddValue <= MaxHealth)
		Health += AddValue;
	else Health = MaxHealth;
}


void ARageBaseCar::Death()
{
	bDead = true;

	SetActorHiddenInGame(true);
	CharacterStatusWidget->Deactivate();
	if (MainWeapon)MainWeapon->SetActorHiddenInGame(true);
	if (AltWeapon)AltWeapon->SetActorHiddenInGame(true);

	BP_Death();
	Global_Death();

	

	if (bCanRevive)
	{

		SetActorEnableCollision(false);
		SetActorLocation(FVector::ZeroVector, false, NULL, ETeleportType::TeleportPhysics);
		if(GetMesh())GetMesh()->SetEnableGravity(false);
		//if (GetMesh())GetMesh()->SetCollisionProfileName("NoCollision");
		FTimerHandle MyHandle;
		GetWorldTimerManager().SetTimer(MyHandle, this, &ARageBaseCar::Revive, ReviveTime, false);
	}
	else Destroy();
	//printr("DIE ");

}
void ARageBaseCar::Global_Death_Implementation()
{

}
void ARageBaseCar::Revive()
{
	bDead = false;
	Health = MaxHealth / 2;
	CharacterStatusWidget->Activate();
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	if (GetMesh())GetMesh()->SetEnableGravity(true);
	if (MainWeapon)MainWeapon->SetActorHiddenInGame(false);
	if (AltWeapon)AltWeapon->SetActorHiddenInGame(false);


//	GetMesh()->COllision
	//if (GetMesh())GetMesh()->SetCollisionProfileName("Vehicle");
	//if (GetMesh())GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	BP_Revive();
	Global_Revive();
}
void ARageBaseCar::Global_Revive_Implementation()
{

}


// Get Health Percent
float ARageBaseCar::GetHealthPercent()
{
	if (Health>0 && MaxHealth>0)
	{
		return Health / MaxHealth;
	}
	return 0;
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
	
	DOREPLIFETIME(ARageBaseCar, CharacterName);
	DOREPLIFETIME(ARageBaseCar, CharacterColor);
	DOREPLIFETIME(ARageBaseCar, Health);
	DOREPLIFETIME(ARageBaseCar, bDead);
	DOREPLIFETIME(ARageBaseCar, MainWeapon);
	DOREPLIFETIME(ARageBaseCar, AltWeapon);
	DOREPLIFETIME(ARageBaseCar, UltWeapon);
}