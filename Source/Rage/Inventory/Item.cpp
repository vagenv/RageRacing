// Copyright 2015-2016 Vagen Ayrapetyan

#include "Rage.h"
#include "Inventory/Item.h"

#include "Inventory/Inventory.h"

#include "Vehicle/RagePlayerCar.h"
#include "UnrealNetwork.h"


// Sets default values
AItem::AItem(const class FObjectInitializer& PCIP)
{

}

void AItem::BeginPlay()
{
	Super::BeginPlay();
}

// Get Player Pointer
ARagePlayerCar* AItem::GetPlayer()const
{
	if (ThePlayer)return ThePlayer;
	else return NULL;
}

void AItem::DropThisItemFromInventory()
{
	if (ThePlayer && ThePlayer->TheInventory)
		ThePlayer->TheInventory->ThrowOut(this);
}
void AItem::RemoveThisItemFromInventory()
{
	if (ThePlayer && ThePlayer->TheInventory)
		ThePlayer->TheInventory->RemoveItem(this);
}



// Get World Pointer
UWorld* AItem::GetPlayerWorld()const
{
	if (ThePlayer && ThePlayer->GetWorld())return ThePlayer->GetWorld();
	else return NULL;
}
AItem* AItem::ItemClassToItemRef(TSubclassOf<AItem> TheItemClass)const
{
	if (TheItemClass && TheItemClass->GetDefaultObject<AItem>())
	{
		return TheItemClass->GetDefaultObject<AItem>();
	}
	else return NULL;
}



void AItem::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AItem, ThePlayer);
}