// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Rage.h"
#include "RageBaseCar.h"
#include "RageWheelFront.h"
#include "RageWheelRear.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/TextRenderComponent.h"
#include "Sound/SoundCue.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Vehicles/WheeledVehicleMovementComponent4W.h"
#include "Engine/SkeletalMesh.h"
#include "ActionComponent.h"
#include "Weapon.h"


const FName ARageBaseCar::LookUpBinding("LookUp");
const FName ARageBaseCar::LookRightBinding("LookRight");
const FName ARageBaseCar::EngineAudioRPM("RPM");

#define LOCTEXT_NAMESPACE "VehiclePawn"

ARageBaseCar::ARageBaseCar()
{
	RootComponent=CreateDefaultSubobject<USceneComponent>(TEXT("TheRoot"));
	GetMesh()->AttachParent = RootComponent;
	// Create a spring arm component for our chase camera
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 34.0f));
	SpringArm->SetWorldRotation(FRotator(-20.0f, 0.0f, 0.0f));
	SpringArm->AttachParent=GetMesh();
	SpringArm->TargetArmLength = 125.0f;
	SpringArm->bEnableCameraLag = false;
	SpringArm->bEnableCameraRotationLag = false;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = true;

	// Create the chase camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ChaseCamera"));
	Camera->AttachTo(SpringArm, USpringArmComponent::SocketName);
	Camera->SetRelativeRotation(FRotator(10.0f, 0.0f, 0.0f));
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 90.f;

	// Create In-Car camera component 
	InternalCameraOrigin = FVector(-34.0f, 0.0f, 50.0f);
	InternalCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("InternalCamera"));
	//InternalCamera->AttachTo(SpringArm, USpringArmComponent::SocketName);
	InternalCamera->bUsePawnControlRotation = false;
	InternalCamera->FieldOfView = 90.f;
	InternalCamera->SetRelativeLocation(InternalCameraOrigin);
	InternalCamera->AttachTo(GetMesh());


	// Setup the audio component and allocate it a sound cue
	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCue(TEXT("/Game/VehicleAdv/Sound/Engine_Loop_Cue.Engine_Loop_Cue"));
	EngineSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineSound"));
	EngineSoundComponent->SetSound(SoundCue.Object);
	EngineSoundComponent->AttachTo(GetMesh());

	TheActionComponent = CreateDefaultSubobject<UActionComponent>(TEXT("Action Component"));

	/*
	MainWeaponMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponStaticMesh"));
	AltWeaponMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponStaticMesh"));

	*/

	if (TheActionComponent)
	{
		/*
		if (MainWeaponMeshComponent && TheActionComponent->MainWeapon && TheActionComponent->MainWeapon->TheStaticMeshComponent
			&& TheActionComponent->MainWeapon->TheStaticMeshComponent->StaticMesh)
			MainWeaponMeshComponent->SetStaticMesh(TheActionComponent->MainWeapon->TheStaticMeshComponent->StaticMesh);

		if (AltWeaponMeshComponent && TheActionComponent->AltWeapon && TheActionComponent->AltWeapon->TheStaticMeshComponent
			&& TheActionComponent->AltWeapon->TheStaticMeshComponent->StaticMesh)
			AltWeaponMeshComponent->SetStaticMesh(TheActionComponent->MainWeapon->TheStaticMeshComponent->StaticMesh);

			*/

	}


}

void ARageBaseCar::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &ARageBaseCar::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ARageBaseCar::MoveRight);
	InputComponent->BindAxis(LookUpBinding);
	InputComponent->BindAxis(LookRightBinding);
	InputComponent->BindAction("Handbrake", IE_Pressed, this, &ARageBaseCar::OnHandbrakePressed);
	InputComponent->BindAction("Handbrake", IE_Released, this, &ARageBaseCar::OnHandbrakeReleased);
	InputComponent->BindAction("SwitchCamera", IE_Pressed, this, &ARageBaseCar::OnToggleCamera);



	if (!TheActionComponent)return;
	InputComponent->BindAction("Action", IE_Pressed, TheActionComponent, &UActionComponent::ActionDown);
	InputComponent->BindAction("Action", IE_Released, TheActionComponent, &UActionComponent::ActionUp);

	InputComponent->BindAction("AltAction", IE_Pressed, TheActionComponent, &UActionComponent::AltActionDown);
	InputComponent->BindAction("AltAction", IE_Released, TheActionComponent, &UActionComponent::AltActionUp);

	InputComponent->BindAction("Boost", IE_Pressed, TheActionComponent, &UActionComponent::BoostDown);
	InputComponent->BindAction("Boost", IE_Released, TheActionComponent, &UActionComponent::BoostUp);



}



void ARageBaseCar::MoveForward(float Val)
{
	GetVehicleMovementComponent()->SetThrottleInput(Val);

}

void ARageBaseCar::MoveRight(float Val)
{
	GetVehicleMovementComponent()->SetSteeringInput(Val);
}

void ARageBaseCar::OnHandbrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void ARageBaseCar::OnHandbrakeReleased()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void ARageBaseCar::OnToggleCamera()
{
	EnableIncarView(!bInCarCameraActive);
}

void ARageBaseCar::EnableIncarView(const bool bState)
{
	if (bState != bInCarCameraActive)
	{
		bInCarCameraActive = bState;
		
		if (bState == true)
		{
			Camera->Deactivate();
			InternalCamera->Activate();
			
			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			if ( (PlayerController != nullptr) && (PlayerController->PlayerCameraManager != nullptr ) )
			{
				PlayerController->PlayerCameraManager->bFollowHmdOrientation = true;
			}
		}
		else
		{
			InternalCamera->Deactivate();
			Camera->Activate();
		}
	}
}

void ARageBaseCar::Tick(float Delta)
{

	Super::Tick(Delta);
	// Setup the flag to say we are in reverse gear
	bInReverseGear = GetVehicleMovement()->GetCurrentGear() < 0;
	
	// Update phsyics material
	UpdatePhysicsMaterial();


	// Pass the engine RPM to the sound component
	float RPMToAudioScale = 2500.0f / GetVehicleMovement()->GetEngineMaxRotationSpeed();
	EngineSoundComponent->SetFloatParameter(EngineAudioRPM, GetVehicleMovement()->GetEngineRotationSpeed()*RPMToAudioScale);
}

void ARageBaseCar::BeginPlay()
{
	Super::BeginPlay();
	bool bWantInCar = false;
	// First disable both speed/gear displays
	bInCarCameraActive = false;

	
	EnableIncarView(bWantInCar);
	// Start an engine sound playing
	EngineSoundComponent->Play();
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
