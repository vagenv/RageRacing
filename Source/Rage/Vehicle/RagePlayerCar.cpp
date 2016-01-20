// Copyright 2015-2016 Vagen Ayrapetyan

#include "Rage.h"
#include "Engine.h"

#include "Vehicle/RagePlayerCar.h"
#include "GameFramework/PlayerController.h"
#include "Weapons/Weapon.h"
#include "Inventory/Item.h"
#include "Inventory/Inventory.h"
#include "Inventory/ItemPickup.h"

#include "RageHUD.h"

#include "UnrealNetwork.h"
#include "System/RageGameState.h"
#include "System/RagePlayerState.h"
#include "System/RageDamageType.h"

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


	// Set Vertical input - Up/Down
	FRichCurve* RichVInputCurve = VerticalInputCurve.GetRichCurve();
	if (RichVInputCurve)
	{
		RichVInputCurve->AddKey(-10, 1);
		RichVInputCurve->AddKey(4.5, 1);
		RichVInputCurve->AddKey(6, -1);
		RichVInputCurve->AddKey(10, -1);
	}

	// Set Horizontal Input- Left/Right
	FRichCurve* RichHInputCurve = HorizontalInputCurve.GetRichCurve();
	if (RichHInputCurve)
	{
		RichHInputCurve->AddKey(3.5, 1);
		RichHInputCurve->AddKey(-3.5, -1);
	}

	// Set Default Character Name
	CharacterName = "Player";


}


//			Begin Play
void ARagePlayerCar::BeginPlay()
{
	Super::BeginPlay();
	bThirdPersonCamera = true;
	if (Role>=ROLE_Authority)
	{
		if (!TheInventory)
		{
			TheInventory = GetWorld()->SpawnActor<UInventory>();
		}
			
		// Add Default Inventory Item
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
		{
			TheHUD = Cast<ARageHUD>(ThePC->GetHUD());
		}
			

	}

	// Start Energy Restore
	StartBoostEnergyRestore();

	// Start Shield Energy Restore
	StartShieldEnergyRestore();

}

// Post begin play
void ARagePlayerCar::PostBeginPlay()
{
	Super::PostBeginPlay();
	// Get Player Controller
	if (!ThePC && GetController() && Cast<APlayerController>(GetController()))
	{
		ThePC = Cast<APlayerController>(GetController());
	}

	// Get HUD Reference
	if (!TheHUD && ThePC && ThePC->GetHUD() && Cast<ARageHUD>(ThePC->GetHUD()))
		TheHUD = Cast<ARageHUD>(ThePC->GetHUD());

}

//		 The Tick
void ARagePlayerCar::Tick(float Delta)
{
	Super::Tick(Delta);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//								Additional Events/Control

bool ARagePlayerCar::AddChatMessage_Validate(const FString & TheMessage)
{
	return true;
}
void ARagePlayerCar::AddChatMessage_Implementation(const FString & TheMessage)
{
	if (GetWorld() && GetWorld()->GetGameState<ARageGameState>())
	{
		GetWorld()->GetGameState<ARageGameState>()->AddNewChatMessage(TheMessage,this);
	}
}

bool ARagePlayerCar::SetPlayerStats_Validate(const FString & newPlayerName, FLinearColor newPlayerColor, FVector newAdditonalColorProperties)
{
	return true;
}

void ARagePlayerCar::SetPlayerStats_Implementation(const FString & newPlayerName, FLinearColor newPlayerColor, FVector newAdditonalColorProperties)
{
	if (PlayerState)
	{
		PlayerState->PlayerName = newPlayerName;
		if (Cast<ARagePlayerState>(PlayerState))Cast<ARagePlayerState>(PlayerState)->PlayerColor = newPlayerColor;
	}
	CharacterName = newPlayerName;
	CharacterColor = newPlayerColor;
	AdditonalColorProperties = newAdditonalColorProperties;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//								Shield Energy Control

// Start Energy Restore
void ARagePlayerCar::StartShieldEnergyRestore()
{
	// Only On Server
	if (Role < ROLE_Authority)return;

	GetWorldTimerManager().SetTimer(ShieldEnergyRestoreHandle, this, &ARagePlayerCar::ShieldEnergy_Restore, TheShieldEnergyData.RestoreSpeed, true);

	// Clear timer to start restore
	if (GetWorldTimerManager().IsTimerActive(ShieldEnergyRestoreStartDelayHandle))
	{
		GetWorldTimerManager().ClearTimer(ShieldEnergyRestoreStartDelayHandle);
	}
}

// Stop Energy Restore
void ARagePlayerCar::StopShieldEnergyRestore()
{
	// Only On Server
	if (Role < ROLE_Authority)return;

	if (GetWorldTimerManager().IsTimerActive(ShieldEnergyRestoreHandle))
		GetWorldTimerManager().ClearTimer(ShieldEnergyRestoreHandle);

	// Clear Timer to start restore
	if (GetWorldTimerManager().IsTimerActive(ShieldEnergyRestoreStartDelayHandle))
	{
		GetWorldTimerManager().ClearTimer(ShieldEnergyRestoreStartDelayHandle);
	}

	// Start delay from zero again.
	GetWorldTimerManager().SetTimer(ShieldEnergyRestoreStartDelayHandle, this, &ARagePlayerCar::StartShieldEnergyRestore, TheShieldEnergyData.TimeBeforeEnergyRestoreRestart, false);

}

// Automatic Energy Restore
void ARagePlayerCar::ShieldEnergy_Restore()
{
	// Only On Server
	if (Role < ROLE_Authority)return;

	if (TheShieldEnergyData.CurrentValue < TheShieldEnergyData.MaxValue)
	{
		if (TheShieldEnergyData.CurrentValue + TheShieldEnergyData.RestoreValue <= TheShieldEnergyData.MaxValue)
		{
			TheShieldEnergyData.CurrentValue += TheShieldEnergyData.RestoreValue;
		}
		else TheShieldEnergyData.CurrentValue = TheShieldEnergyData.MaxValue;

		BP_ShieldEnergyValueUpdated();
	}
	
}


// Take Damage override
void ARagePlayerCar::ShieldAbsorbDamage(float& DamageAmount, struct FDamageEvent const& DamageEvent)
{
	StopShieldEnergyRestore();


	if (TheShieldEnergyData.CurrentValue <= 0)return;

	// Total Dmg absorbed from dmg
	float TotalAbsorbValue = 0;

	URageDamageType* RageDmg = DamageEvent.DamageTypeClass->GetDefaultObject<URageDamageType>();

	// Total Penetration Dmg
	float PenetrationDmg = 0;


	// Rage Dmg
	if (RageDmg)
	{

		PenetrationDmg = DamageAmount*RageDmg->Damage_ShieldPenetrationMultiplier;
		// Total Dmg to shield
		float ShieldTotalDmg = DamageAmount*RageDmg->Damage_ShieldDamageMultiplier-PenetrationDmg;

		// Shield can absorb that dmg
		if (TheShieldEnergyData.CurrentValue > ShieldTotalDmg)
		{
			//printg("everything Absorbed");

			// Take out Shield energy
			TheShieldEnergyData.CurrentValue -= ShieldTotalDmg;

			// Absorbed everthing
			TotalAbsorbValue = DamageAmount;
		}
		// Shield Can't absorb everything
		else
		{

			//printg("Part Absorbed");
			// Take out the damage that shield can absorb
			ShieldTotalDmg -= TheShieldEnergyData.CurrentValue;

			// Caculate absorb value
			TotalAbsorbValue = (ShieldTotalDmg / RageDmg->Damage_ShieldDamageMultiplier);
			PenetrationDmg = 0;

			// Set shield energy
			TheShieldEnergyData.CurrentValue = 0;

		}
	}
	// Not Rage Dmg
	else 
	{
		// Shield can absorb that dmg
		if (TheShieldEnergyData.CurrentValue > DamageAmount)
		{
			// Take From Shield
			TheShieldEnergyData.CurrentValue -= DamageAmount;

			// Absorbed All Dmg
			TotalAbsorbValue = DamageAmount;
		}
		// Shield Can't absorb everything
		else
		{
			// Absorb Shield value from dmg
			TotalAbsorbValue = TheShieldEnergyData.CurrentValue;

			// Absorb all Shield Energy
			TheShieldEnergyData.CurrentValue = 0;
		}
	}



	//  Original Damage -  Absorbed Value +  Penetrated Damage
	DamageAmount = DamageAmount- TotalAbsorbValue + PenetrationDmg;
	
	/*
	printb("Damage After Amount :  " + FString::FromInt(DamageAmount));

	printr("Absorb Amount :  " + FString::FromInt(TotalAbsorbValue));

	printr("Penetratio Amount :  " + FString::FromInt(PenetrationDmg));

	printr("Armor Amount :  " + FString::FromInt(TheShieldEnergyData.CurrentValue));
	*/

	BP_ShieldEnergyValueUpdated();
}




// Use Specific energy value
void ARagePlayerCar::UseShieldEnergy(float Value)
{
	TheShieldEnergyData.CurrentValue -= Value;

	if (TheShieldEnergyData.CurrentValue < 0)TheShieldEnergyData.CurrentValue = 0;

	
}

// Get Current Energy Percent
float ARagePlayerCar::GetShieldEnergyPercent()
{
	if (TheShieldEnergyData.CurrentValue > 0 && TheShieldEnergyData.MaxValue>0)
	{
		return TheShieldEnergyData.CurrentValue / TheShieldEnergyData.MaxValue;
	}
	return 0;
}

// Client Shield Energy Updated
void ARagePlayerCar::OnRep_ShieldEnergyReplicated()
{
	BP_ShieldEnergyValueUpdated();
}





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//								Boost Energy Control

// Start Energy Restore
void ARagePlayerCar::StartBoostEnergyRestore(){
	GetWorldTimerManager().SetTimer(BoostEnergyRestoreHandle, this, &ARagePlayerCar::BoostEnergy_Restore, TheBoostEnergyData.RestoreSpeed, true);
}

// Stop Energy Restore
void ARagePlayerCar::StopBoostEnergyRestore(){
	if (GetWorldTimerManager().IsTimerActive(BoostEnergyRestoreHandle))
		GetWorldTimerManager().ClearTimer(BoostEnergyRestoreHandle);
}

// Automatic Energy Restore
void ARagePlayerCar::BoostEnergy_Restore(){
	TheBoostEnergyData.RestoreEnergy();
}


// Use all energy Value
void ARagePlayerCar::UseAllBoostEnergy()
{
	TheBoostEnergyData.UseAllEnergy();
}

// Use Specific energy value
void ARagePlayerCar::UseBoostEnergy(float Value)
{
	TheBoostEnergyData.UseEnergy(Value);
}


// Get Current Energy Percent
float ARagePlayerCar::GetBoostEnergyPercent()
{
	if (TheBoostEnergyData.CurrentValue >= 0 && TheBoostEnergyData.MaxValue>0)
	{
		return TheBoostEnergyData.CurrentValue / TheBoostEnergyData.MaxValue;
	}
	return 0;
}

// Check if can boos
bool ARagePlayerCar::CanBoost()
{
	//if (!VehicleMovement->IsMovingOnGround())return false;
	if (!TheBoostEnergyData.CanBoost()) return false;

	return true;
}

// Check if can boost Jump
bool ARagePlayerCar::CanJump()
{
	//if (!VehicleMovement->IsMovingOnGround())return false;
	if (!TheBoostEnergyData.CanJump())return false;
	return true;
}

// Client Boost Energy Updated
void ARagePlayerCar::OnRep_BoostEnergyReplicated()
{

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//								Weapon Events and Functions



// Equip Default Weapon
void ARagePlayerCar::EquipDefaultWeapons()
{
	Super::EquipDefaultWeapons();

	if (Role >= ROLE_Authority)
	{
		// Equip Default Main Weapon
		if (DefaultMainWeapon && DefaultMainWeapon->GetDefaultObject<AWeapon>())
		{
			if (TheInventory)TheInventory->AddItem(DefaultMainWeapon);
			EquipNewWeapon(DefaultMainWeapon->GetDefaultObject<AWeapon>());
		
		}
		// Equip Default Alt Weapon
		if (DefaultAltWeapon)
		{

			if (TheInventory)TheInventory->AddItem(DefaultAltWeapon);
			EquipNewWeapon(DefaultAltWeapon->GetDefaultObject<AWeapon>());
		}
	}
	
	// Update Inventory Status
	InventoryUpdated();
}




// Weapon Updated
void ARagePlayerCar::OnRep_WeaponsUpdated()
{
	if (TheHUD)TheHUD->BP_InventoryUpdated();
	if (MainWeapon)MainWeapon->ClientWeaponUpdated();
	if (AltWeapon)AltWeapon->ClientWeaponUpdated();
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
	else if (MainWeapon->bEquiped)
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

// One of Weapons Ammo Finished

void ARagePlayerCar::WeaponAmmoFinished()
{

	float delay = WeaponSwitchDelay+0.1f;
	
	if (MainWeapon)delay += MainWeapon->EquipTime;
	else if (AltWeapon)delay += MainWeapon->EquipTime;
	else delay += 0.5f;

	FTimerHandle MyHandle;
	GetWorldTimerManager().SetTimer(MyHandle, this, &ARagePlayerCar::SearchNewWeaponEquip, delay, false);
}
// Search if any weapon to equip is avaiable
void ARagePlayerCar::SearchNewWeaponEquip()
{

	//printg("Searching new weapon");
	if (TheInventory && TheInventory->Items.Num() > 0)
	{
		/*
		for (int32 i = 0; i < TheInventory->Items.Num();i++)
		{

		}*/

		// Search new main Weapon
		if (!MainWeapon)
		{
		
			for (auto IterItem:TheInventory->Items)
			{
				if (IterItem.Archetype->GetDefaultObject<AWeapon>() && IterItem.Archetype->GetDefaultObject<AWeapon>()->WeaponType==EWeaponArchetype::Main)
				{
					EquipNewWeapon(IterItem.Archetype->GetDefaultObject<AWeapon>());
					//printr("New Main Weapon Found");
					InventoryUpdated();
					break;
				}
			}

		}


		// Search new alt Weapon
		if (!AltWeapon)
		{

			for (auto IterItem : TheInventory->Items)
			{
				if (IterItem.Archetype->GetDefaultObject<AWeapon>() && IterItem.Archetype->GetDefaultObject<AWeapon>()->WeaponType == EWeaponArchetype::Secondary)
				{
					EquipNewWeapon(IterItem.Archetype->GetDefaultObject<AWeapon>());
					//printr("New Alternative Weapon Found");
					InventoryUpdated();
					break;
				}
			}

		}


		/*
		//printb("Search new Weapon");
		if (MainWeapon == NULL || AltWeapon == NULL)
		{
		for (int32 i = 0; i < ItemList.Num();i++)
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
		//InventoryUpdated();

	}

	
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//								Inventory Events and Functions


bool ARagePlayerCar::Server_UseItem_Validate(AItem* TheItem)
{
	return (TheItem == NULL) ? false : true;
}
void ARagePlayerCar::Server_UseItem_Implementation(AItem* TheItem)
{
	//printg("Server -> Item Used");
	if (TheItem) TheItem->InventoryUse(this);
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


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//								Camera	 Component

// Toggle Current Camera
void ARagePlayerCar::OnToggleCamera()
{
	if (bThirdPersonCamera == true)
	{
		InternalCamera->Deactivate();
		Camera->Activate();
		bThirdPersonCamera = false;
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
		bThirdPersonCamera = true;
		Camera->Deactivate();
		InternalCamera->Activate();
	}

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//								Death Revive and DMG




// Take Damage override
float ARagePlayerCar::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{

	printr("Before Absorb " + FString::SanitizeFloat(DamageAmount));


	// Absorb the dmg from shield
	ShieldAbsorbDamage(DamageAmount, DamageEvent);

	printg("After Absorb " + FString::SanitizeFloat(DamageAmount));

	//print("");

	// Get Vehicle Dmg Multiplier
	URageDamageType* RageDmg = DamageEvent.DamageTypeClass->GetDefaultObject<URageDamageType>();
	if (RageDmg){
		DamageAmount *= RageDmg->Damage_VehicleDamageMultiplier;
	}

	return Super::TakeDamage(DamageAmount,DamageEvent,EventInstigator,DamageCauser);
};


// Player Died
void ARagePlayerCar::Death()
{
	Super::Death();

	// Disable Input
	if (ThePC)
	{
		DisableInput(ThePC);
	}
//	GetMesh()->Deactivate();
}

// Revive Player
void ARagePlayerCar::Revive()
{
	Super::Revive();
	if (ThePC)
	{
		EnableInput(ThePC);
	}
	//GetMesh()->Activate();

	// Find The Any Revive Point
	TActorIterator<APlayerStart> p(GetWorld());
	APlayerStart* revivePoint = *p;

	// Check if any other revive points are closer
	for (TActorIterator<APlayerStart> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (revivePoint && FVector::Dist(revivePoint->GetActorLocation(), GetActorLocation())> FVector::Dist(ActorItr->GetActorLocation(), GetActorLocation()))
		{
			revivePoint = *ActorItr;
		}
	}

	// Create a small offset from the spawn point
	if (revivePoint)
		SetActorLocation(revivePoint->GetActorLocation() + FVector(FMath::RandRange(-400, 400), FMath::RandRange(-400, 400), 60), false, NULL, ETeleportType::TeleportPhysics);
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//								State Checking


// If car is in state to fire
bool ARagePlayerCar::CanShoot()
{
	return true;
}



// Check if car is turned over
bool ARagePlayerCar::IsTurnedOver()
{
	if (FMath::Abs(GetActorRotation().Roll) > TurnedOverValidAngle && GetVelocity().Size()<TurnedOverValidVelocity)return true;

	return false;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//								Input Binding and Events

// Input Setup
void ARagePlayerCar::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	check(InputComponent);

	// If Android -> Enable Android Input
	if (PLATFORM_ANDROID)
		InputComponent->BindVectorAxis("Gravity", this, &ARagePlayerCar::AddGravityMotionInput);

	// If Windows enable keyboard input
	if (PLATFORM_WINDOWS)
	{
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
}

// Android Motion Input
void ARagePlayerCar::AddGravityMotionInput(FVector Val)
{
	if (Val != FVector::ZeroVector)
	{
		// Get Tablet Orientation->   Landscape/ Inverted Landscape
		if (Val.X > 4) ANDROID_Tilt_OrientationValue = 1;
		else if (Val.X < -4)ANDROID_Tilt_OrientationValue = -1;

		//Horizontal Motion Input
		if (HorizontalInputCurve.GetRichCurve())
		{
			AddHorizontalInput(HorizontalInputCurve.GetRichCurve()->Eval(Val.Y*ANDROID_Tilt_OrientationValue));
		}
		//Vertical Motion Input
		if (HorizontalInputCurve.GetRichCurve())
		{
			AddVerticalInput(VerticalInputCurve.GetRichCurve()->Eval(Val.X*ANDROID_Tilt_OrientationValue));
		}
	}
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

bool ARagePlayerCar::AddInput_FireStart_Validate()
{
	return true;
}

void ARagePlayerCar::AddInput_FireStart_Implementation()
{
	FireStart();
}

bool ARagePlayerCar::AddInput_FireEnd_Validate()
{
	return true;
}
void ARagePlayerCar::AddInput_FireEnd_Implementation()
{
	FireEnd();
}

bool ARagePlayerCar::AddInput_AltFireStart_Validate()
{
	return true;
}
void ARagePlayerCar::AddInput_AltFireStart_Implementation()
{
	AltFireStart();
}

bool ARagePlayerCar::AddInput_AltFireEnd_Validate()
{
	return true;
}
void ARagePlayerCar::AddInput_AltFireEnd_Implementation()
{
	AltFireEnd();
}

bool ARagePlayerCar::AddInput_Boost_Validate()
{
	return true;
}

void ARagePlayerCar::AddInput_Boost_Implementation()
{
	if (CanBoost())Boost();
}

bool ARagePlayerCar::AddInput_JumpBoost_Validate()
{
	return true;
}

void ARagePlayerCar::AddInput_JumpBoost_Implementation()
{
	if (CanJump())DoubleJump();
}

// Boost Event
void ARagePlayerCar::Boost()
{
	BP_Boost();

}

// Jump Boost Event
void ARagePlayerCar::DoubleJump()
{
	if (!CanJump())return;

	BP_DoubleJump();

	// Get Vertical Direction
	FVector TheJumpImpulse = GetActorUpVector();// FVector(0); //GetVelocity();

	// Set Vertical Impulse Value
	TheJumpImpulse.Z *= TheBoostEnergyData.CurrentValue *TheBoostEnergyData.JumpMultiplier*TheBoostEnergyData.JumpDirection.Y;

	// Set Forward Impulse Value
	TheJumpImpulse += GetActorForwardVector()*TheBoostEnergyData.CurrentValue*TheBoostEnergyData.JumpMultiplier*TheBoostEnergyData.JumpDirection.X;

	// Add The Impulse
	GetMesh()->AddImpulse(TheJumpImpulse, NAME_None, true);

	// Use Energy
	UseAllBoostEnergy();
}

// Reset Car event 
bool ARagePlayerCar::ResetCar_Validate()
{
	return IsTurnedOver();
}
void ARagePlayerCar::ResetCar_Implementation()
{
	if (!IsTurnedOver())return;

	BP_CarWasReseted();

	FTransform CarTransform = GetTransform();
	CarTransform.SetLocation(CarTransform.GetLocation() + FVector(0, 0, ResetRestoreHeight));
	CarTransform.SetRotation(FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw, 0).Quaternion());
	SetActorTransform(CarTransform, false, NULL, ETeleportType::TeleportPhysics);
	if (GetVehicleMovement())GetVehicleMovement()->Velocity = FVector(0);

}



// Replication of data
void ARagePlayerCar::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARagePlayerCar, TheBoostEnergyData);
	DOREPLIFETIME(ARagePlayerCar, TheShieldEnergyData);
	DOREPLIFETIME(ARagePlayerCar, TheInventory);
	DOREPLIFETIME(ARagePlayerCar, AdditonalColorProperties);
}