// Fill out your copyright notice in the Description page of Project Settings.

#include "Rage.h"
#include "Engine.h"


#include "Vehicle/RagePlayerCar.h"
#include "Weapons/Weapon.h"
#include "Inventory/Item.h"
#include "RageHUD.h"


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

	MainWeapon = NULL;
	AltWeapon = NULL;

	EnableIncarView(bWantInCar);

	EquipDefaultWeapons();
	//FTimerHandle MyHandle;
	//GetWorldTimerManager().SetTimer(MyHandle, this, &ARagePlayerCar::EquipDefaultWeapons,0.2f,false);


	StartEneryRestore();

}


void ARagePlayerCar::EquipDefaultWeapons()
{

	if (DefaultMainWeapon)
	{
		MainWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultMainWeapon);
		MainWeapon->AttachRootComponentTo(GetMesh(), MainWeapon->AttachSocket);
		MainWeapon->TheCar = this;
		ItemList.Add(MainWeapon);

	}
	if (DefaultAltWeapon)
	{
		AltWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultAltWeapon);
		AltWeapon->AttachRootComponentTo(GetMesh(), MainWeapon->AttachSocket);
		AltWeapon->TheCar = this;
		ItemList.Add(AltWeapon);
	}

	InventoryUpdated();
}


void ARagePlayerCar::StartEneryRestore()
{
	GetWorldTimerManager().SetTimer(EnergyRestoreHandle, this, &ARagePlayerCar::Energy_Restore, Energy_RestoreSpeed, true);

}

void ARagePlayerCar::StopEneryRestore()
{
	if (GetWorldTimerManager().IsTimerActive(EnergyRestoreHandle))
	{
		GetWorldTimerManager().ClearTimer(EnergyRestoreHandle);
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

	InputComponent->BindAction("NextAction", IE_Pressed, this, &ARagePlayerCar::NextAction);
	InputComponent->BindAction("NextAltAction", IE_Pressed, this, &ARagePlayerCar::NextAltAction);
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

	if (CanBoost())
		GetWorldTimerManager().SetTimer(BoostDelayTimerHandle, this, &ARagePlayerCar::Boost, Energy_BoostJumpSelectDelay, false);
}
void ARagePlayerCar::Boost()
{
	BP_Boost();



	//GetMesh()->AddImpulse(GetActorForwardVector()*Energy_CurrentValue*Energy_BoostMultiplier, NAME_None, true);
	//UseEnergy();


}
void ARagePlayerCar::BoostUp()
{
	BP_BoostUp();
}

void ARagePlayerCar::NextAction()
{
	BP_NextAction();


	TArray<AItem*> TheWeapons = GetMainWeaponList();

	if (MainWeapon && TheWeapons.Num()>1)
	{
		if (TheWeapons.Num()==2)
		{
			// Switch to other Weapon
			if (TheWeapons[0] && MainWeapon->GetClass() == TheWeapons[0]->GetClass())
			{
				UnequipWeapon((uint8)EWeaponArchetype::Main);



				EquipNewWeapon(Cast<AWeapon>(TheWeapons[1]));
			}
			else if (TheWeapons[0] && MainWeapon->GetClass() == TheWeapons[1]->GetClass())
			{
				UnequipWeapon((uint8)EWeaponArchetype::Main);
				EquipNewWeapon(Cast<AWeapon>(TheWeapons[0]));
			}
		}
		else 
		{
			// More versions

			for (int i = 0; i < TheWeapons.Num();i++)
			{
				if (TheWeapons.IsValidIndex(i) && TheWeapons[i])
				{

				}
			}

		}


	}

	/*
	for (int i = 0; i < ItemList.Num();i++)
	{
		if (ItemList.IsValidIndex(i) && ItemList[i])
		{
			printg(ItemList[i]->Name);
		}
	}
	*/
}

void ARagePlayerCar::NextAltAction()
{
	BP_NextAltAction();
}

void ARagePlayerCar::DoubleJump()
{
	if (GetWorldTimerManager().IsTimerActive(BoostDelayTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(BoostDelayTimerHandle);
	}


	if (!CanJump())return;

	BP_DoubleJump();


	FVector TheJumpImpulse = GetActorUpVector();// FVector(0); //GetVelocity();

	
	TheJumpImpulse.Z *= Energy_CurrentValue*Energy_JumpMultiplier*Energy_JumpDirection.Y;


	TheJumpImpulse += GetActorForwardVector()*Energy_CurrentValue*Energy_JumpMultiplier*Energy_JumpDirection.X;


	GetMesh()->AddImpulse(TheJumpImpulse, NAME_None, true);	

	UseAllEnergy();
	/*
	*/
}


void ARagePlayerCar::UseAllEnergy()
{
	Energy_CurrentValue = 0;
}
void ARagePlayerCar::UseEnergy(float Value)
{
	if (Energy_CurrentValue<Value)
	{
		Energy_CurrentValue = 0;
	}
	else Energy_CurrentValue -=Value;
}



bool ARagePlayerCar::CanBoost()
{
	//if (!VehicleMovement->IsMovingOnGround())return false;
	if (Energy_CurrentValue < Energy_BoostMinValue) return false;

	return true;
}

bool ARagePlayerCar::CanJump()
{
	//if (!VehicleMovement->IsMovingOnGround())return false;
	if (Energy_CurrentValue < Energy_BoostJumpMinValue) return false;
	return true;
}




void ARagePlayerCar::EquipNewWeapon(AWeapon* TheWeapon)
{
	//printg("Equip new Weapon"+TheWeapon->Name);
	if (TheWeapon->WeaponType==EWeaponArchetype::Main && MainWeapon==NULL)
	{
		MainWeapon = GetWorld()->SpawnActor<AWeapon>(TheWeapon->GetClass());
		MainWeapon->AttachRootComponentTo(GetMesh(), MainWeapon->AttachSocket);
		MainWeapon->TheCar = this;
		if (!ItemList.Contains(TheWeapon))
			ItemList.Add(TheWeapon);
		
	}
	else if (TheWeapon->WeaponType == EWeaponArchetype::Secondary && AltWeapon == NULL)
	{

		AltWeapon = GetWorld()->SpawnActor<AWeapon>(TheWeapon->GetClass());
		AltWeapon->AttachRootComponentTo(GetMesh(), AltWeapon->AttachSocket);
		AltWeapon->TheCar = this;
		if (!ItemList.Contains(TheWeapon))
			ItemList.Add(TheWeapon);
	}

	BP_InventoryUpdated();
}

void ARagePlayerCar::UnequipWeapon(uint8 WeaponType)
{
	//printr("Unequip Weapon");
	if (EWeaponArchetype(WeaponType) == EWeaponArchetype::Main && MainWeapon)
	{
		/*
		if (MainWeapon->FireCost > 0 && MainWeapon->CurrentAmmo < MainWeapon->FireCost)
		{
			for (int i = 0; i < ItemList.Num();i++)
			{
				if (ItemList.IsValidIndex(i) && MainWeapon->GetClass() == ItemList[i]->GetClass())
				{
					ItemList.RemoveAt(i);
					break;
				}
			}
		}
		*/
		MainWeapon->Destroy();
		MainWeapon = NULL;

	}
	if (EWeaponArchetype(WeaponType) == EWeaponArchetype::Secondary && AltWeapon)
	{
		if (AltWeapon->HasAmmo())
		{
			/*
			//AltWeapon->FireCost > 0 && AltWeapon->CurrentAmmo < AltWeapon->FireCost
			for (int i = 0; i < ItemList.Num(); i++)
			{
				if (ItemList.IsValidIndex(i) && AltWeapon->GetClass() == ItemList[i]->GetClass())
				{
					ItemList.RemoveAt(i);
					break;
				}
			}
			*/
		}
		AltWeapon->Destroy();
		AltWeapon = NULL;
	}


	InventoryUpdated();



	FTimerHandle MyHandle;
	GetWorldTimerManager().SetTimer(MyHandle, this, &ARagePlayerCar::SearchNewWeaponEquip, 1, false);
}
void ARagePlayerCar::SearchNewWeaponEquip()
{
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

	InventoryUpdated();
}




void ARagePlayerCar::InventoryUpdated()
{
	//print("Update HUD");
	BP_InventoryUpdated();
	if (TheHUD)TheHUD->BP_InventoryUpdated();
	//else printr("NO HUD");
}

void ARagePlayerCar::ItemPickup(TSubclassOf<class AItem>  TheItem)
{

	bool bNewItem = true;

	AItem* TheItemPtr = TheItem->GetDefaultObject<AItem>();
	// If stacking item
	// Weapon Stacking
	
	if (TheItemPtr && Cast<AWeapon>(TheItemPtr))
	{
		AWeapon* NewWeapon = Cast<AWeapon>(TheItemPtr);

		for (int i = 0; i < ItemList.Num(); i++)
		{
			if (ItemList.IsValidIndex(i) && Cast<AWeapon>(ItemList[i]) && NewWeapon->GetClass() == ItemList[i]->GetClass())
			{
				Cast<AWeapon>(ItemList[i])->AddAmmoPickup(NewWeapon);
				bNewItem = false;
				break;
			}

		}
	}
	/*
	*/



	// If new Item
	if (bNewItem)
	{
		ItemList.Add(TheItem->GetDefaultObject<AItem>());
	}
	TheItemPtr->BP_ItemPickedUp(this);


	//  If no current weapon equip new obtained
	if (!MainWeapon || !AltWeapon)
	{
		AWeapon* TempWeapon = TheItem->GetDefaultObject<AWeapon>();
		if (TempWeapon)
		{
			if (TempWeapon->WeaponType == EWeaponArchetype::Main && !MainWeapon)
			{

				MainWeapon = GetWorld()->SpawnActor<AWeapon>(TheItem);
				MainWeapon->AttachRootComponentTo(GetMesh(), MainWeapon->AttachSocket);
				MainWeapon->TheCar = this;
			}
			else if (TempWeapon->WeaponType == EWeaponArchetype::Secondary && !AltWeapon)
			{
				AltWeapon = GetWorld()->SpawnActor<AWeapon>(TheItem);
				AltWeapon->AttachRootComponentTo(GetMesh(), AltWeapon->AttachSocket);
				AltWeapon->TheCar = this;
			}
		}
	}
	



	BP_ItemPickedUp(TheItemPtr, TheItem);
	InventoryUpdated();

}
TArray<AItem* > ARagePlayerCar::GetAllItemList()
{
	return ItemList;
}

TArray<AItem* > ARagePlayerCar::GetOnlyItemList()
{
	TArray<AItem*> OnlyItems;
	for (int i = 0; i < ItemList.Num(); i++)
	{
		if (ItemList.IsValidIndex(i) && ItemList[i] && !Cast<AWeapon>(ItemList[i]))
		{
			OnlyItems.Add(ItemList[i]);
		}
	}
	return OnlyItems;
}

TArray<AItem* > ARagePlayerCar::GetMainWeaponList()
{
	TArray<AItem*> WeaponList;

	for (int i = 0; i < ItemList.Num();i++)
	{
		if (ItemList.IsValidIndex(i) && ItemList[i] && Cast<AWeapon>(ItemList[i]) 
			 && Cast<AWeapon>(ItemList[i])->WeaponType== EWeaponArchetype::Main)
		{
			WeaponList.Add(ItemList[i]);
		}
	}

	return WeaponList;
}
TArray<AItem* > ARagePlayerCar::GetAltWeaponList()
{
	TArray<AItem*> WeaponList;

	for (int i = 0; i < ItemList.Num(); i++)
	{
		if (ItemList.IsValidIndex(i) && ItemList[i] && Cast<AWeapon>(ItemList[i])
			&& Cast<AWeapon>(ItemList[i])->WeaponType == EWeaponArchetype::Secondary)
		{
			WeaponList.Add(ItemList[i]);
		}
	}

	return WeaponList;
}
void ARagePlayerCar::SortInventory()
{

}
