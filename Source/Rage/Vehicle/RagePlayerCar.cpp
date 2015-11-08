// Copyright 2015 Vagen Ayrapetyan

#include "Rage.h"
#include "Engine.h"


#include "Vehicle/RagePlayerCar.h"
#include "Weapons/Weapon.h"
#include "Inventory/Item.h"
#include "Inventory/Inventory.h"
#include "Inventory/ItemPickup.h"

#include "RageHUD.h"

#include "UnrealNetwork.h"


#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/TextRenderComponent.h"


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
	InternalCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("InternalCamera"));
	InternalCamera->bUsePawnControlRotation = false;
	InternalCamera->FieldOfView = 90.f;
	InternalCamera->SetRelativeLocation(FVector(-34.0f, 0.0f, 50.0f));
	InternalCamera->AttachTo(GetMesh());


	TheInventory = CreateDefaultSubobject<UInventory>(TEXT("TheInventory"));
	TheInventory->SetIsReplicated(true);

}


// Begin Play
void ARagePlayerCar::BeginPlay()
{
	Super::BeginPlay();
	
	if (Role>=ROLE_Authority)
	{
		if (!TheInventory)
		{
			printr("No Inventory");
			TheInventory = GetWorld()->SpawnActor<UInventory>();
		}
			
	
		if (TheInventory)
		{
			TheInventory->ThePlayer = this;

			for (int32 i = 0; i < DefaultInventoryItems.Num(); i++)
			{
				if (DefaultInventoryItems.IsValidIndex(i))
					TheInventory->AddItem(DefaultInventoryItems[i]);
			}
		}

		


		// Get Player Controller
		if (GetController() && Cast<APlayerController>(GetController()))
		{
			ThePC = Cast<APlayerController>(GetController());
		}

		// Get HUD Reference
		if (ThePC && ThePC->GetHUD() && Cast<ARageHUD>(ThePC->GetHUD()))
			TheHUD = Cast<ARageHUD>(ThePC->GetHUD());

	}

	StartEneryRestore();

	FTimerHandle MyHandle;
	GetWorldTimerManager().SetTimer(MyHandle, this, &ARagePlayerCar::PostBeginPlay, 1, false);

}

void ARagePlayerCar::PostBeginPlay()
{
	// Get Player Controller
	if (!ThePC && GetController() && Cast<APlayerController>(GetController()))
	{
		ThePC = Cast<APlayerController>(GetController());
	}

	// Get HUD Reference
	if (!TheHUD && ThePC && ThePC->GetHUD() && Cast<ARageHUD>(ThePC->GetHUD()))
		TheHUD = Cast<ARageHUD>(ThePC->GetHUD());
}

// If car is in state to fire
bool ARagePlayerCar::CanShoot()
{
	return true;
}

// Equip Default Weapon
void ARagePlayerCar::EquipDefaultWeapons()
{
	Super::EquipDefaultWeapons();

	if (Role >= ROLE_Authority)
	{
		if (DefaultMainWeapon && DefaultMainWeapon->GetDefaultObject<AWeapon>())
		{
			if (TheInventory)TheInventory->AddItem(DefaultMainWeapon);
			EquipNewWeapon(DefaultMainWeapon->GetDefaultObject<AWeapon>());
		
		}
		if (DefaultAltWeapon)
		{

			if (TheInventory)TheInventory->AddItem(DefaultAltWeapon);
			EquipNewWeapon(DefaultAltWeapon->GetDefaultObject<AWeapon>());
		}
	}
	
	InventoryUpdated();
}

// Start Energy Restore
void ARagePlayerCar::StartEneryRestore(){
	GetWorldTimerManager().SetTimer(EnergyRestoreHandle, this, &ARagePlayerCar::Energy_Restore, TheEnergyData.RestoreSpeed, true);
}

// Stop Energy Restore
void ARagePlayerCar::StopEneryRestore(){
	if (GetWorldTimerManager().IsTimerActive(EnergyRestoreHandle))
		GetWorldTimerManager().ClearTimer(EnergyRestoreHandle);
}

// Automatic Energy Restore
void ARagePlayerCar::Energy_Restore(){
	TheEnergyData.RestoreEnergy();
}


// Input Setup
void ARagePlayerCar::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &ARagePlayerCar::AddVerticalInput);
	InputComponent->BindAxis("MoveRight", this, &ARagePlayerCar::AddHorizontalInput);


	InputComponent->BindAction("SwitchCamera", IE_Pressed, this, &ARagePlayerCar::OnToggleCamera);

	InputComponent->BindAction("Fire", IE_Pressed, this, &ARagePlayerCar::FireStart);
	InputComponent->BindAction("Fire", IE_Released, this, &ARagePlayerCar::FireEnd);

	InputComponent->BindAction("AltFire", IE_Pressed, this, &ARagePlayerCar::AltFireStart);
	InputComponent->BindAction("AltFire", IE_Released, this, &ARagePlayerCar::AltFireEnd);


	InputComponent->BindAction("Boost", IE_Pressed, this, &ARagePlayerCar::Boost);
	InputComponent->BindAction("Boost", IE_DoubleClick, this, &ARagePlayerCar::DoubleJump);

}


// Add Vertical Input
void ARagePlayerCar::AddVerticalInput(float Val)
{
	GetVehicleMovementComponent()->SetThrottleInput(Val);

}

// Add Horizontal
void ARagePlayerCar::AddHorizontalInput(float Val)
{
	GetVehicleMovementComponent()->SetSteeringInput(Val);
}


// Toggle Current Camera
void ARagePlayerCar::OnToggleCamera()
{
	if (bThirdPersonCamera == true)
	{
		InternalCamera->Deactivate();
		Camera->Activate();

		/*
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if ((PlayerController != nullptr) && (PlayerController->PlayerCameraManager != nullptr))
		{
			PlayerController->PlayerCameraManager->bFollowHmdOrientation = true;
		}
		*/
	}
	else
	{
		Camera->Deactivate();
		InternalCamera->Activate();
	}

}


// Start Main Weapon Fire
void ARagePlayerCar::FireStart()
{
	if (MainWeapon)MainWeapon->FireStart();
}

// End Main Weapon Fire
void ARagePlayerCar::FireEnd()
{
	if (MainWeapon)MainWeapon->FireEnd();
}

// Start Alt Weapon Fire
void ARagePlayerCar::AltFireStart()
{
	if (AltWeapon)AltWeapon->FireStart();
}

// End Alt Weapon Fire
void ARagePlayerCar::AltFireEnd()
{
	if (AltWeapon)AltWeapon->FireEnd();
}


// Inputs called from outside
void ARagePlayerCar::AddInput_FireStart()
{
	FireStart();
}
void ARagePlayerCar::AddInput_FireEnd()
{
	FireEnd();
}
void ARagePlayerCar::AddInput_AltFireStart()
{
	AltFireStart();
}
void ARagePlayerCar::AddInput_AltFireEnd()
{
	AltFireEnd();
}
///

// Boost Event
void ARagePlayerCar::Boost()
{
	BP_Boost();

	/*
	if (CanBoost())
		GetWorldTimerManager().SetTimer(BoostDelayTimerHandle, this, &ARagePlayerCar::Boost, TheEnergyData.BoostJumpSelectDelay, false);
		*/
}

// Jump Boost Event
void ARagePlayerCar::DoubleJump()
{
	if (GetWorldTimerManager().IsTimerActive(BoostDelayTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(BoostDelayTimerHandle);
	}


	if (!CanJump())return;

	BP_DoubleJump();


	FVector TheJumpImpulse = GetActorUpVector();// FVector(0); //GetVelocity();

	
	TheJumpImpulse.Z *= TheEnergyData.CurrentValue *TheEnergyData.JumpMultiplier*TheEnergyData.JumpDirection.Y;


	TheJumpImpulse += GetActorForwardVector()*TheEnergyData.CurrentValue*TheEnergyData.JumpMultiplier*TheEnergyData.JumpDirection.X;


	GetMesh()->AddImpulse(TheJumpImpulse, NAME_None, true);	

	UseAllEnergy();
}

// Use all energy Value
void ARagePlayerCar::UseAllEnergy()
{
	TheEnergyData.UseAllEnergy();
}

// Use Specific energy value
void ARagePlayerCar::UseEnergy(float Value)
{
	TheEnergyData.UseEnergy(Value);
}

// Get Current Energy Percent
float ARagePlayerCar::GetEnergyPercent()
{
	if (TheEnergyData.CurrentValue >= 0 && TheEnergyData.MaxValue>0)
	{
		return TheEnergyData.CurrentValue / TheEnergyData.MaxValue;
	}
	return 0;
}

void ARagePlayerCar::ResetCar()
{
	if (!IsTurnedOver())return;

	BP_CarWasReseted();

	FTransform CurrentTransform = GetTransform();

}
/*
*/


// Check if can boos
bool ARagePlayerCar::CanBoost()
{
	//if (!VehicleMovement->IsMovingOnGround())return false;
	if (!TheEnergyData.CanBoost()) return false;

	return true;
}

// Check if can boost Jump
bool ARagePlayerCar::CanJump()
{
	//if (!VehicleMovement->IsMovingOnGround())return false;
	if (!TheEnergyData.CanJump())return false;
	return true;
}



// Equip New Weapon
void ARagePlayerCar::EquipNewWeapon(AWeapon* TheWeapon)
{
	if (!TheWeapon)return;


	// No Main Weapon, Equip if new is main weapon
	if (!MainWeapon)
	{
		if (TheWeapon->WeaponType == EWeaponArchetype::Main && MainWeapon == NULL)
		{
			MainWeapon = GetWorld()->SpawnActor<AWeapon>(TheWeapon->GetClass());
			MainWeapon->ThePlayer = this;
			MainWeapon->EquipStart();
			//MainWeapon->AttachWeapon();

		}
	}
	// If this weapon is equiped, unequip
	else if (MainWeapon->bEquiped )
	{
		// Same Weapon unequip it
		if (MainWeapon->GetClass() == TheWeapon->GetClass())
		{
			UnEquipWeapon(EWeaponArchetype::Main);
		}
		else if (TheWeapon->WeaponType == EWeaponArchetype::Main && !PendingWeaponEquip)
		{
			UnEquipWeapon(EWeaponArchetype::Main);

			PendingWeaponEquip = TheWeapon;

			FTimerHandle MyHandle;
			GetWorldTimerManager().SetTimer(MyHandle, this, &ARagePlayerCar::EquipPendingWeapon, MainWeapon->EquipTime + WeaponSwitchDelay, false);
		}

	}
	

	// No Alt Weapon, Equip if new is alt weapon
	if (!AltWeapon)
	{
		if (TheWeapon->WeaponType == EWeaponArchetype::Secondary && AltWeapon == NULL)
		{
			AltWeapon = GetWorld()->SpawnActor<AWeapon>(TheWeapon->GetClass());
			AltWeapon->ThePlayer = this;
			AltWeapon->EquipStart();
			//AltWeapon->AttachWeapon();

		}
	}

	// If this weapon is equiped, unequip
	else if (AltWeapon->bEquiped)
	{
		if (AltWeapon->GetClass() == TheWeapon->GetClass())
		{
			UnEquipWeapon(EWeaponArchetype::Secondary);
		}
	

		else if (TheWeapon->WeaponType == EWeaponArchetype::Secondary && !PendingWeaponEquip)
		{
			UnEquipWeapon(EWeaponArchetype::Secondary);

			PendingWeaponEquip = TheWeapon;

			FTimerHandle MyHandle;
			GetWorldTimerManager().SetTimer(MyHandle, this, &ARagePlayerCar::EquipPendingWeapon, AltWeapon->EquipTime + WeaponSwitchDelay, false);
		}
	}
		

	BP_InventoryUpdated();
}

// Equip pending weapon
void ARagePlayerCar::EquipPendingWeapon()
{
	if (!PendingWeaponEquip)return;

	EquipNewWeapon(PendingWeaponEquip);
	PendingWeaponEquip = NULL;
}

// Unequip current weapon
void ARagePlayerCar::UnEquipWeapon(EWeaponArchetype WeaponType)
{

	if (WeaponType == EWeaponArchetype::Main && MainWeapon)
	{
		MainWeapon->UnEquipStart();
	}
	else if (WeaponType == EWeaponArchetype::Secondary && AltWeapon)
	{
		AltWeapon->UnEquipStart();
	}

}

// Search if any weapon to equip is avaiable
void ARagePlayerCar::SearchNewWeaponEquip()
{

	/*
	//printb("Search new Weapon");
	if (MainWeapon == NULL || AltWeapon == NULL)
	{
		for (int i = 0; i < ItemList.Num();i++)
		{
			if (ItemList.IsValidIndex(i) && Cast<AWeapon>(ItemList[i]) )
			{
				if (MainWeapon == NULL && Cast<AWeapon>(ItemList[i])->WeaponType == EWeaponArchetype::Main)
				{
					EquipNewWeapon(Cast<AWeapon>(ItemList[i]));
				}
				if (AltWeapon == NULL  &&Cast<AWeapon>(ItemList[i])->WeaponType == EWeaponArchetype::Secondary)
				{
					EquipNewWeapon(Cast<AWeapon>(ItemList[i]));
				}
			}
		}
	}
	*/
	InventoryUpdated();
}

// Pickup the selected item
void ARagePlayerCar::PickupTheItemPickup()
{
	if (currentPickup)
	{
		currentPickup->PickedUp(this);
	}
}

// Inventory Updated
void ARagePlayerCar::InventoryUpdated()
{
	BP_InventoryUpdated();
	if (TheHUD)TheHUD->BP_InventoryUpdated();
}

// Check if car is turned over
bool ARagePlayerCar::IsTurnedOver()
{
	if (FMath::Abs(GetActorRotation().Roll) > TurnedOverValidAngle && GetVelocity().Size()<TurnedOverValidVelocity)return true;

	return false;
}



// Replication of data
void ARagePlayerCar::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARagePlayerCar, TheEnergyData);
	DOREPLIFETIME(ARagePlayerCar, TheInventory);


}