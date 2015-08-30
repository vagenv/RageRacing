// Fill out your copyright notice in the Description page of Project Settings.

#include "Rage.h"
#include "RagePlayerCar.h"
#include "Engine.h"
#include "Weapon.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/TextRenderComponent.h"



const FName ARagePlayerCar::LookUpBinding("LookUp");
const FName ARagePlayerCar::LookRightBinding("LookRight");

ARagePlayerCar::ARagePlayerCar()
{
	// Create a spring arm component for our chase camera
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 34.0f));
	SpringArm->SetWorldRotation(FRotator(-20.0f, 0.0f, 0.0f));
	SpringArm->AttachParent = GetMesh();
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
}

void ARagePlayerCar::BeginPlay()
{
	Super::BeginPlay();

	bool bWantInCar = false;
	// First disable both speed/gear displays
	bInCarCameraActive = false;


	EnableIncarView(bWantInCar);



	FTimerHandle MyHandle;
	GetWorldTimerManager().SetTimer(MyHandle, this, &ARagePlayerCar::Energy_Restore, Energy_RestoreSpeed, true);

	if (DefaultMainWeapon)
	{

		MainWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultMainWeapon);
		MainWeapon->AttachRootComponentTo(GetMesh(), MainWeapon->AttachSocket);
		MainWeapon->TheCar = this;
		//MainWeapon->AttachActorToActor
		//MainWeapon->AttachTo(TheCar->GetMesh(), "MainWeaponSocket");

	}
	if (DefaultAltWeapon)
	{

		AltWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultAltWeapon);
		AltWeapon->AttachRootComponentTo(GetMesh(), AltWeapon->AttachSocket);
		AltWeapon->TheCar = this;
		//AltWeapon->AttachTo(TheCar->GetMesh(), "AltWeaponSocket");
	}



}





void ARagePlayerCar::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &ARagePlayerCar::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ARagePlayerCar::MoveRight);
	InputComponent->BindAxis(LookUpBinding);
	InputComponent->BindAxis(LookRightBinding);
	InputComponent->BindAction("Handbrake", IE_Pressed, this, &ARagePlayerCar::OnHandbrakePressed);
	InputComponent->BindAction("Handbrake", IE_Released, this, &ARagePlayerCar::OnHandbrakeReleased);
	InputComponent->BindAction("SwitchCamera", IE_Pressed, this, &ARagePlayerCar::OnToggleCamera);


	InputComponent->BindAction("Action", IE_Pressed, this, &ARagePlayerCar::ActionDown);
	InputComponent->BindAction("Action", IE_Released, this, &ARagePlayerCar::ActionUp);

	InputComponent->BindAction("AltAction", IE_Pressed, this, &ARagePlayerCar::AltActionDown);
	InputComponent->BindAction("AltAction", IE_Released, this, &ARagePlayerCar::AltActionUp);

	InputComponent->BindAction("Boost", IE_Pressed, this, &ARagePlayerCar::BoostDown);
	InputComponent->BindAction("Boost", IE_Released, this, &ARagePlayerCar::BoostUp);

	InputComponent->BindAction("Boost", IE_DoubleClick, this, &ARagePlayerCar::DoubleJump);
	/*
	*/
}



void ARagePlayerCar::MoveForward(float Val)
{
	GetVehicleMovementComponent()->SetThrottleInput(Val);

}

void ARagePlayerCar::MoveRight(float Val)
{
	GetVehicleMovementComponent()->SetSteeringInput(Val);
}

void ARagePlayerCar::OnHandbrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void ARagePlayerCar::OnHandbrakeReleased()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void ARagePlayerCar::OnToggleCamera()
{
	EnableIncarView(!bInCarCameraActive);
}

void ARagePlayerCar::EnableIncarView(const bool bState)
{
	if (bState != bInCarCameraActive)
	{
		bInCarCameraActive = bState;

		if (bState == true)
		{
			Camera->Deactivate();
			InternalCamera->Activate();

			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			if ((PlayerController != nullptr) && (PlayerController->PlayerCameraManager != nullptr))
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

void ARagePlayerCar::Energy_Restore()
{
	if (Energy_CurrentValue<Energy_MaxValue)
	{
		Energy_CurrentValue += Energy_RestoreValue;

		if (Energy_CurrentValue > Energy_MaxValue)
			Energy_CurrentValue = Energy_MaxValue;
	}
}

float ARagePlayerCar::GetEnergyPercent()
{
	if (Energy_CurrentValue >= 0 && Energy_MaxValue>0)
	{
		return Energy_CurrentValue / Energy_MaxValue;
	}

	return 0;
}



void ARagePlayerCar::ActionDown()
{
	BP_ActionDown();
	if (MainWeapon)
	{
		MainWeapon->FireStart();
	}
}
void ARagePlayerCar::ActionUp()
{
	BP_ActionUp();
	if (MainWeapon)
	{
		MainWeapon->FireEnd();
	}
}


void ARagePlayerCar::AltActionDown()
{
	BP_AltActionDown();
	if (AltWeapon)
	{
		AltWeapon->FireStart();
	}
}
void ARagePlayerCar::AltActionUp()
{
	BP_AltActionUp();
	if (AltWeapon)
	{
		AltWeapon->FireEnd();
	}
}


void ARagePlayerCar::BoostDown()
{
	BP_BoostDown();
}
void ARagePlayerCar::BoostUp()
{
	BP_BoostUp();
}

void ARagePlayerCar::DoubleJump()
{
	BP_DoubleJump();
}