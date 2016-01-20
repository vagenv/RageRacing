// Copyright 2015-2016 Vagen Ayrapetyan

#include "Rage.h"
#include "Engine.h"

#include "Inventory/Inventory.h"
#include "Weapons/Weapon.h"
#include "Vehicle/RagePlayerCar.h"
#include "UnrealNetwork.h"


AWeapon::AWeapon(const class FObjectInitializer& PCIP) :AItem(PCIP)
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("TheRoot"));

	TheStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	TheStaticMeshComponent->AttachParent = RootComponent;
	bReplicates = true;

}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	// Double check at the begining
	bReloading = false;
	bShooting = false;
	bEquiped = false;
}

// Player Pressed Fire
void AWeapon::FireStart()
{
	// BP Fire Started
	BP_FirePress_Start();

	// If not currently in main and alternative shooting mode/delay  call The Fire event
	if (ThePlayer && !ThePlayer->GetWorldTimerManager().IsTimerActive(PreFireTimeHandle))
		ThePlayer->GetWorldTimerManager().SetTimer(PreFireTimeHandle, this, &AWeapon::PreFire, WeaponData.FireSpeed, true, 0);

	// Shooting Enabled
	bShooting = true;

}

// Player Released Fire
void AWeapon::FireEnd()
{
	// BP Fire Released
	BP_FirePress_End();

	// Shooting Disabled
	bShooting = false;
}

// Pre Fire
void AWeapon::PreFire()
{
	// BP Pre Fire
	BP_PreFire();

	if (!ThePlayer)return;

	// If player stopped shooting stop event
	if (!bShooting)
	{
		ThePlayer->GetWorldTimerManager().ClearTimer(PreFireTimeHandle);
		return;
	}

	// Currently Equiping this weapon, delay a bit

	
	if (!bEquiped)
	{
		// try after a small delay
		FTimerHandle MyHandle;
		ThePlayer->GetWorldTimerManager().SetTimer(MyHandle, this, &AWeapon::PreFire, 0.2, false);
		return;
	}
	/*
	*/


	// Wrong Player Anim State
	if (!CanShoot())return;

	// Ammo Check
	if (!WeaponData.CanFire())
	{
		//  BP No Ammo
		BP_NoAmmo();
		UnEquipStart();
		if (ThePlayer)ThePlayer->WeaponAmmoFinished();
		RemoveThisItemFromInventory();
		return;
	}

	// The real fire event
	Fire();

	// Use Ammo
	if (bUseAmmo)UseAmmo();
}


// Weapon Used in Inventory
void AWeapon::InventoryUse(ARagePlayerCar* Player)
{
	Super::InventoryUse(Player);

	if (Player)Player->EquipNewWeapon(this);

}

// Equip Start
void AWeapon::EquipStart()
{
	if (!ThePlayer)return;

	// Bp Equip Start
	BP_Equip_Start();


	AttachWeapon();

	// Load Weapon Data from inventory
	if (ThePlayer && ThePlayer->TheInventory)
	{
		for (int32 i = 0; i < ThePlayer->TheInventory->Items.Num(); i++)
		{

			if (ThePlayer->TheInventory->Items.IsValidIndex(i) &&
				ThePlayer->TheInventory->Items[i].Archetype &&
				ThePlayer->TheInventory->Items[i].Archetype->GetDefaultObject() &&
				ThePlayer->TheInventory->Items[i].Archetype->GetDefaultObject()->GetClass() == GetClass())
			{

				WeaponData = ThePlayer->TheInventory->Items[i].TheWeaponData;
				break;
			}

		}
	}

	// Delay equip end
	FTimerHandle myHandle;
	ThePlayer->GetWorldTimerManager().SetTimer(myHandle, this, &AWeapon::EquipEnd, EquipTime, false);

	bEquiped = false;
}

// Equip Ended
void AWeapon::EquipEnd()
{
	// BP equip Ended
	BP_Equip_End();


	bEquiped = true;
	// Save Inventory
	if (ThePlayer && ThePlayer->TheInventory)
	{
		ThePlayer->TheInventory->UpdateInfo();
		ThePlayer->TheInventory->SaveInventory();
	}
}

// Unequip Start
void AWeapon::UnEquipStart()
{

	// BP UnEquip Start
	BP_Unequip_Start();

	bEquiped = false;


	// Save Weapon Data to inventory
	if (ThePlayer && ThePlayer->TheInventory)
	{
		for (int32 i = 0; i < ThePlayer->TheInventory->Items.Num(); i++)
		{

			if (ThePlayer->TheInventory->Items.IsValidIndex(i) &&
				ThePlayer->TheInventory->Items[i].Archetype &&
				ThePlayer->TheInventory->Items[i].Archetype->GetDefaultObject() &&
				ThePlayer->TheInventory->Items[i].Archetype->GetDefaultObject()->GetClass() == GetClass())
			{

				ThePlayer->TheInventory->Items[i].TheWeaponData=WeaponData;
				break;
			}

		}

	}

	//delay unequip End
	FTimerHandle myHandle;
	ThePlayer->GetWorldTimerManager().SetTimer(myHandle, this, &AWeapon::UnEquipEnd, EquipTime, false);

	// Save the weapon stats
	SaveCurrentWeaponStats();

}

// Unequip End
void AWeapon::UnEquipEnd()
{
	// BP unequip End
	BP_Unequip_End();

	

	if (ThePlayer)
	{
		// Clear Weapon Ref
		if (ThePlayer->MainWeapon == this)ThePlayer->MainWeapon = NULL;
		if (ThePlayer->AltWeapon == this)ThePlayer->AltWeapon = NULL;

		// Save Inventory
		if (ThePlayer && ThePlayer->TheInventory)
		{
			ThePlayer->TheInventory->SaveInventory();
			ThePlayer->TheInventory->UpdateInfo();
		} 
	}





	// Destroy the Weapon Actor
	Destroy();
}



void AWeapon::ClientWeaponUpdated()
{
	AttachWeapon();
}

void AWeapon::AttachWeapon()
{

	if (ThePlayer)
	{
		if (AttachSocketName.ToString().Len()<1)
		{
			switch (WeaponType)
			{
				case EWeaponArchetype::Main:
				{
					AttachSocketName = "MainWeaponSocket";
					break;
				}
				case EWeaponArchetype::Secondary:
				{
					AttachSocketName = "AltWeaponSocket";
					break;
				}
				case EWeaponArchetype::Ultimate:
				{
					AttachSocketName = "UltimateWeaponSocket";
					break;
				}
			}
		}

		TheStaticMeshComponent->AttachTo(ThePlayer->GetMesh(), AttachSocketName);
	}

}

// Can Weapon and Player Fire
bool AWeapon::CanShoot()
{
	if (!ThePlayer || !ThePlayer->CanShoot() || !bEquiped)return false;

	return true;
}


// Save Weapon Data
void AWeapon::SaveCurrentWeaponStats()
{
	if (!ThePlayer || !ThePlayer->TheInventory)return;

	// Find Weapon data in item list
	for (int32 i = 0; i < ThePlayer->TheInventory->Items.Num(); i++)
	{

		if (ThePlayer->TheInventory->Items.IsValidIndex(i) && ThePlayer->TheInventory->Items[i].Archetype->GetDefaultObject()->GetClass() == GetClass())
		{
			// Set The Inventory item data to this weapon data
			ThePlayer->TheInventory->Items[i].TheWeaponData = WeaponData;
			return;
		}
	}
}

// Use Fire Ammo
void AWeapon::UseAmmo()
{
	// BP Ammo Used
	BP_Ammo_Used();

	// Substract Fire Cost
	WeaponData.CurrentAmmo -= WeaponData.FireCost;
}

// Add Ammo from weapon pointer
void AWeapon::AddAmmo(AWeapon* weapAmmo)
{
	if (weapAmmo)
	{
		WeaponData.AddAmmo(weapAmmo->WeaponData.CurrentAmmo);
	}
}

// Add Ammo as a number
void AWeapon::AddAmmo(float newAmmo)
{
	WeaponData.AddAmmo(newAmmo);
}

void AWeapon::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeapon, WeaponData);
}