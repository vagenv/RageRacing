// Copyright 2015-2016 Vagen Ayrapetyan


#include "Rage.h"
#include "RageHUD.h"
#include "Inventory/Item.h"
#include "Vehicle/RagePlayerCar.h"



ARageHUD::ARageHUD()
{

}

void ARageHUD::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle MyHandle;
	GetWorldTimerManager().SetTimer(MyHandle, this, &ARageHUD::PostBeginPlay, PostDelay,false);

}

void ARageHUD::PostBeginPlay()
{
	if (GetOwningPawn() && Cast<ARagePlayerCar>(GetOwningPawn()))
	{
		Cast<ARagePlayerCar>(GetOwningPawn())->TheHUD = this;
	}
	else 
	{
		FTimerHandle MyHandle;
		GetWorldTimerManager().SetTimer(MyHandle, this, &ARageHUD::PostBeginPlay, PostDelay, false);
	}
}

ARagePlayerCar* ARageHUD::GetPlayer()
{
	return Cast<ARagePlayerCar>(GetOwningPawn());
}

AItem* ARageHUD::ItemClassToItemRef(TSubclassOf<AItem> TheItemClass)const
{
	if (TheItemClass && TheItemClass->GetDefaultObject<AItem>())
	{
		return TheItemClass->GetDefaultObject<AItem>();
	}
	else return NULL;
}
