// Copyright 2015 Vagen Ayrapetyan

#include "Rage.h"
#include "RageData.h"
#include "Weapons/Weapon.h"
#include "System/RageGameInstance.h"
#include "Vehicle/RagePlayerCar.h"



FRageOnineMessageData::FRageOnineMessageData(FString NewMessage, ARagePlayerCar* ThePlayer)
{

	MessageTime = FDateTime::Now();
	TheMessage = NewMessage;
	if (ThePlayer)
	{
		MessageOwner = ThePlayer->CharacterName;
		MessageColor = ThePlayer->CharacterColor;
	}
	

}

FRageOninePlayerData::FRageOninePlayerData(ARagePlayerCar* NewPlayer)
{
	if (!NewPlayer)return;

	ThePlayer = NewPlayer;

	PlayerName = NewPlayer->CharacterName;

	PlayerColor = NewPlayer->CharacterColor;

	CurrentHealth=NewPlayer->Health;

}


FAvaiableSessionsData::FAvaiableSessionsData(FOnlineSessionSearchResult newSessionData)
{
	SessionData = newSessionData;

	OwnerName = newSessionData.Session.OwningUserName;
	Ping = newSessionData.PingInMs;
	NumberOfConnections = newSessionData.Session.SessionSettings.NumPublicConnections;
	NumberOfAvaiableConnections = NumberOfConnections-newSessionData.Session.NumOpenPublicConnections;
}


FItemData::FItemData(TSubclassOf<AItem> Item, FString newItemName, UTexture2D* newItemIcon, float newWeight, int32 newItemCount)
{

	// Set Item Data
	Archetype = Item;
	ItemName = newItemName;
	ItemIcon = newItemIcon;
	ItemCount = newItemCount;
	Weight = newWeight;

	// if Weapon Set Main and Alt Fire
	if (Item && Item->GetDefaultObject<AWeapon>())
	{
		TheWeaponData = Item->GetDefaultObject<AWeapon>()->WeaponData;

	}
}


void FEnergyData::RestoreEnergy()
{
	if (CurrentValue<MaxValue)
	{
		CurrentValue += RestoreValue;

		if (CurrentValue > MaxValue)
			CurrentValue = MaxValue;
	}
}
bool FEnergyData::CanBoost()
{
	if (CurrentValue < BoostMinValue)return false;
	else return true;
}
bool FEnergyData::CanJump()
{
	if (CurrentValue < BoostJumpMinValue)return false;
	else return true;
}
void FEnergyData::UseAllEnergy()
{
	CurrentValue = 0;
}
void FEnergyData::UseEnergy(float Value)
{
	if (CurrentValue<Value)
	{
		CurrentValue = 0;
	}
	else CurrentValue -= Value;
}


// Set Item Data from other data
void FItemData::SetItemData(FItemData newData)
{
	ItemName = newData.ItemName;

	ItemIcon = newData.ItemIcon;

	ItemCount = newData.ItemCount;

	Weight = newData.Weight;

	Archetype = newData.Archetype;

	TheWeaponData = newData.TheWeaponData;
}



// Add Ammo To Fire Stats
void FWeaponData::AddAmmo(float newAmmo)
{
	if (newAmmo>0)
	{
		if (CurrentAmmo + newAmmo <= MaxAmmo) CurrentAmmo += newAmmo;
		else CurrentAmmo = MaxAmmo;
	}
}

// Can Fire
bool FWeaponData::CanFire()
{
	if (FireCost > 0 && FireCost > CurrentAmmo)return false;
	return true;
}


