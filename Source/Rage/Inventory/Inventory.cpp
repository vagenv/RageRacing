// Copyright 2015-2016 Vagen Ayrapetyan

#include "Rage.h"
#include "Engine.h"

#include "RageGameMode.h"
#include "RageHUD.h"

#include "Vehicle/RagePlayerCar.h"

#include "Inventory/Inventory.h"
#include "Inventory/ItemPickup.h"
#include "Weapons/Weapon.h"

#include "System/SystemSaveGame.h"
#include "UnrealNetwork.h"


UInventory::UInventory()
{
	bWantsBeginPlay = true;
}


void UInventory::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);

	if (!ThePlayer&& Cast<ARagePlayerCar>(GetOwner()))
		ThePlayer = Cast<ARagePlayerCar>(GetOwner());
}

// Use Item With Index
void UInventory::ActionIndex(int32 ItemIndex)
{
	if (Items.IsValidIndex(ItemIndex) && Items[ItemIndex].Archetype && Items[ItemIndex].Archetype->GetDefaultObject<AItem>())
	{
		Action(Items[ItemIndex].Archetype->GetDefaultObject<AItem>());
	}

}

// Use Item With Ref
void UInventory::Action(AItem* ItemRef)
{
	if (!ItemRef || !ThePlayer)
		return;

	// Call Events in the BP
	ItemRef->InventoryUse(ThePlayer);
	ThePlayer->BP_ItemUsed(ItemRef);

}


// Thow out item with index
void UInventory::ThrowOutIndex(int32 ItemIndex)
{

	// Inside the list of items
	if (Items.IsValidIndex(ItemIndex) && ThePlayer)
	{
		// Check if current weapon, can't throw out weapon that is equiped
		/*
		if (Items[ItemIndex].Archetype && ThePlayer->TheWeapon
			&& ThePlayer->TheWeapon->GetClass() == Items[ItemIndex].Archetype.GetDefaultObject()->GetClass())
		{
			return;
		}
		*/

		UWorld* const World = ThePlayer->GetWorld();
		if (World && Items[ItemIndex].Archetype && Items[ItemIndex].Archetype->GetDefaultObject<AItem>())
		{
			// Get Player Rotation
			FRotator rot = ThePlayer->GetActorRotation();
			FVector spawnLoc =  GetOwner()->GetActorLocation()+FVector(0,0,100);


			AItemPickup* newPickup;
			AItem* newItem = Items[ItemIndex].Archetype->GetDefaultObject<AItem>();

			// Spawn Item Pickup archtype
			if (newItem && newItem->ItemPickupArchetype)
				newPickup = World->SpawnActor<AItemPickup>(newItem->ItemPickupArchetype, spawnLoc, rot);

			// Spawn Default pickup
			else newPickup = World->SpawnActor<AItemPickup>(AItemPickup::StaticClass(), spawnLoc, rot);

			if (newPickup)
			{
				newPickup->SetReplicates(true);
				if (newItem)
				{
					newItem->BP_ItemDroped(newPickup);

					newPickup->Item = newItem->GetClass();
					if (newItem->PickupMesh)
					{
						newPickup->Mesh->SetStaticMesh(newItem->PickupMesh);
					}
					/*
					else if (newItem->PickupSkelMesh)
					{
						newPickup->SkeletalMesh->SetSkeletalMesh(newItem->PickupSkelMesh);
					}
					*/

				}
				if (newPickup->Mesh)
				{
					newPickup->Mesh->SetSimulatePhysics(true);
					newPickup->Mesh->WakeRigidBody();

				}
				/*
				else if (newPickup->SkeletalMesh)
				{
					newPickup->SkeletalMesh->SetSimulatePhysics(true);
					newPickup->SkeletalMesh->WakeRigidBody();
				}
				*/
				newPickup->bAutoPickup = true;
				if (Cast<AWeapon>(newItem))
				{
					newPickup->bOverideItemData = true;
					newPickup->OverideItemData = Items[ItemIndex];

				}


				if (Items[ItemIndex].ItemCount > 1)
				{
					Items[ItemIndex].ItemCount--;
				}
				else RemoveItemIndex(ItemIndex);


				newPickup->ActivatePickupPhysics();
				if (newPickup->Mesh  && newPickup->Mesh->IsSimulatingPhysics())
				{
					newPickup->Mesh->AddImpulse(rot.Vector() * 12000, NAME_None, true);
				}
				/*
				if (newPickup->SkeletalMesh)
				{
					newPickup->SkeletalMesh->AddForce(rot.Vector() * 16000, NAME_None, true);
				}
				*/
				UpdateInfo();
			}
		}
	}
}


// Throw ou item by pointer
void UInventory::ThrowOut(AItem* ItemRef)
{
	// Find the item from items list
	for (int32 i = 0; i < Items.Num(); i++)
	{
		if (Items.IsValidIndex(i) && Items[i].Archetype.GetDefaultObject()->GetClass() == ItemRef->GetClass())
		{
			ThrowOutIndex(i);
			return;
		}
	}
}

// Item Picked up from the pickup
void UInventory::ItemPickedUp(AItemPickup* ThePickup)
{
	if (!ThePickup || !ThePickup->Item)
		return;


	FItemData* NewData = AddItem(ThePickup->Item);
	
	// If New Data Was added
	if (NewData)
	{
	
		//  Override Item data?
		if (ThePickup->bOverideItemData)
				NewData->SetItemData(ThePickup->OverideItemData);

		// Get Item ref
		if (NewData->Archetype)
		{
			AItem* TheItem = NewData->Archetype->GetDefaultObject<AItem>();
			//Call pickup Event on item
			if (TheItem)
				TheItem->BP_ItemPickedUp(ThePickup);
		}
			
		UpdateInfo();
		
	}
	
	if (ThePlayer)ThePlayer->SearchNewWeaponEquip();
}


// Add subclass of item to inventory 
FItemData* UInventory::AddItem(TSubclassOf<AItem> newItem)
{
	if (newItem == NULL)
		return NULL;


	// Check if same item in inventory 
	if (Items.Num()>0)
	{

		// If new Item is weapon and same type as current weapon
		
		if (newItem->GetDefaultObject<AWeapon>() && ThePlayer)
		{
			if (ThePlayer->MainWeapon && ThePlayer->MainWeapon->GetClass() == newItem->GetDefaultObject<AWeapon>()->GetClass())
			{
				// Add Ammo to the current weapon equiped
				ThePlayer->MainWeapon->AddAmmo(newItem->GetDefaultObject<AWeapon>());
				// Find the weapon data and return it.
				for (int32 i = 0; i < Items.Num(); i++)
				{
					if (Items.IsValidIndex(i) && Items[i].Archetype.GetDefaultObject()->GetClass() == ThePlayer->MainWeapon->GetClass())
						return &Items[i];
				}
			}
			else if (ThePlayer->AltWeapon && ThePlayer->AltWeapon->GetClass() == newItem->GetDefaultObject<AWeapon>()->GetClass())
			{
				// Add Ammo to the current weapon equiped
				ThePlayer->AltWeapon->AddAmmo(newItem->GetDefaultObject<AWeapon>());
				// Find the weapon data and return it.
				for (int32 i = 0; i < Items.Num(); i++)
				{
					if (Items.IsValidIndex(i) && Items[i].Archetype.GetDefaultObject()->GetClass() == ThePlayer->AltWeapon->GetClass())
						return &Items[i];
				}
			}
		}
		/*
		*/


		// Check current item list if same item exists
		for (int32 i = 0; i < Items.Num(); i++)
		{

			// If same object archetype
			if (Items[i].Archetype->GetDefaultObject()->GetClass() == newItem->GetDefaultObject()->GetClass())
			{

				if (newItem->GetDefaultObject<AItem>())newItem->GetDefaultObject<AItem>()->BP_ItemUpdated();

				/*
				// IF Weapon add ammo, else add count
				if (Cast<AWeapon>(newItem->GetDefaultObject<AItem>()))
				{
					Items[i].MainFireStats.AddAmmo(newItem->GetDefaultObject<AWeapon>()->MainFire.CurrentAmmo, newItem->GetDefaultObject<AWeapon>()->MainFire.ClipNumber);
				}
				else 
				*/	
				Items[i].ItemCount++;

				// Save and Update Item Data
				SaveInventory();
				UpdateInfo();
				return &Items[i];
			}
		}
	}

	// New Item 
	AItem* newItemBase = newItem->GetDefaultObject<AItem>();
	FItemData newData = FItemData(newItem, newItemBase->ItemName, newItemBase->ItemIcon, newItemBase->Weight, newItemBase->ItemCount);

	// Add New item to item list and update inventory
	Items.Add(newData);
	UpdateInfo();

	return &Items[Items.Num() - 1];

	/*
	*/

}

// Same as AddItem but with value struct return
FItemData UInventory::AddNewItem(TSubclassOf<AItem> newItem)
{
	return *(AddItem(newItem));
}
// Remp with ID
void UInventory::RemoveItemIndex(int32 ItemID)
{
	if (!Items.IsValidIndex(ItemID))return;
	Items.RemoveAt(ItemID);
	UpdateInfo();
}

// Remove Item with ref
void UInventory::RemoveItem(AItem* DeleteItem)
{
	if (!DeleteItem)return;

	
	// Find Item in item list
	for (int32 i = 0; i < Items.Num(); i++)
	{
		if (Items.IsValidIndex(i) && Items[i].Archetype->GetDefaultObject()->GetClass() == DeleteItem->GetClass())
		{
			//printr("Item Found");
			RemoveItemIndex(i);
			break;
		}
			
	}
}


// Inventory Updated
void UInventory::UpdateInfo()
{

	//printg("Update Info");
	//if (!ThePlayer)print
	if (ThePlayer && ThePlayer->TheHUD) 
	{
		ThePlayer->TheHUD->BP_InventoryUpdated();
	} 

	if (Items.Num()<1)return;

	TotalWeight = 0;

	// Calculate weight of each item * itemcound
	for (int32 i = 0; i < Items.Num(); i++)
	{
		if (Items[i].Weight>0 && Items[i].ItemCount>0)
			TotalWeight += (Items[i].Weight*Items[i].ItemCount);
	}
	SaveInventory();
}

// Update Item List, Called on client
void UInventory::OnRep_ItemListUpdated()
{
	if (ThePlayer && ThePlayer->TheHUD)
		ThePlayer->TheHUD->BP_InventoryUpdated();
}

// Load Inventory
void UInventory::LoadInventory()
{
	// Find Game Mode and Save FIle
	if (GetWorld() && GetWorld()->GetAuthGameMode())
	{
		TheGM = GetWorld()->GetAuthGameMode<ARageGameMode>();
		if (TheGM && TheGM->SaveFile)
		{
			Items = TheGM->SaveFile->Items;
		}
	}
	// Error With Game Mode or Save file ref, retry after 0.5 sec
	if (!TheGM || !TheGM->SaveFile && GetOwner())
	{
		FTimerHandle MyHandle;
		GetOwner()->GetWorldTimerManager().SetTimer(MyHandle, this, &UInventory::LoadInventory, 0.5, false);
	}

	// Set Inventory Items data to items data from save file
	if (TheGM && TheGM->SaveFile)Items = TheGM->SaveFile->Items;
}

// Save Inventory
void UInventory::SaveInventory()
{
	// Save current weapon stats
	//if (ThePlayer && ThePlayer->TheWeapon)ThePlayer->TheWeapon->SaveCurrentWeaponStats();

	// Set Savefile items
	if (TheGM && TheGM->SaveFile)TheGM->SaveFile->Items = Items;
}

void UInventory::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UInventory, Items);
	DOREPLIFETIME(UInventory, TotalWeight);
	DOREPLIFETIME(UInventory, ThePlayer);
}