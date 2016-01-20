// Copyright 2015-2016 Vagen Ayrapetyan

#pragma once

#include "Object.h"
#include "RageData.generated.h"




////////////////////////        Shield Data Type
USTRUCT(BlueprintType)
struct FShieldEnergyData
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
		float CurrentValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
		float MaxValue=100;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
		float RestoreSpeed=0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
		float RestoreValue = 1;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
		float TimeBeforeEnergyRestoreRestart = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
		float BaseDmgMutiplier = 0.5f;




};



////////////////////////        Online Message Type
USTRUCT(BlueprintType)
struct FRageOnlineMessageData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
		FString TheMessage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
		FString MessageOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
		FDateTime MessageTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
		FLinearColor MessageColor;


	FRageOnlineMessageData(){};
	FRageOnlineMessageData(FString NewMessage, class ARagePlayerCar * ThePlayer);


};



////////////				Weapon Type
UENUM(BlueprintType)		
enum class EWeaponArchetype : uint8
{
	Main						UMETA(DisplayName = "Main Weapon"),
	Secondary					UMETA(DisplayName = "Secondary Weapon"),
	Ultimate					UMETA(DisplayName = "Ultimate Weapon"),

};




class FOnlineSessionSearchResult;
// Fire State Struct
USTRUCT(BlueprintType)
struct FAvaiableSessionsData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
		FString OwnerName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
		int32 Ping;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
		int32 NumberOfConnections;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
		int32 NumberOfAvaiableConnections;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	FOnlineSessionSearchResult SessionData;

	FAvaiableSessionsData(){};
	FAvaiableSessionsData(class FOnlineSessionSearchResult newSessionData);

};



// Fire State Struct
USTRUCT(BlueprintType)
struct FPlayerSaveData
{
	GENERATED_USTRUCT_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
		FString PlayerName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
		FVector PlayerColor;

	// Player Data
	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	*/

};

// Fire State Struct
USTRUCT(BlueprintType)
struct FBoostEnergyData
{
	GENERATED_USTRUCT_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
		float MaxValue = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
		float CurrentValue = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
		float RestoreSpeed = 0.1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
		float RestoreValue = 1;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
		float BoostMultiplier = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
		float BoostMinValue = 40;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
		float JumpMultiplier = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
		FVector2D JumpDirection = FVector2D(0.5, 1);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
		float BoostJumpSelectDelay = 0.3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
		float BoostJumpMinValue = 50;


	void UseAllEnergy();
	void UseEnergy(float Value);
	void RestoreEnergy();
	bool CanBoost();
	bool CanJump();

};


// Fire State Struct
USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()



	// Numer of bullets in clip
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
		float MaxAmmo = 10;

	// Current number of bullets |  In current Magazine
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
		float CurrentAmmo = 8;

	// Fire Damage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
		float FireDamage = 1;

	// Fire Speed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
		float FireSpeed = 1;

	// Cost of Fire
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
		float FireCost = 1;

	// Fire Distance
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
		float FireDistance = 2500;

	// Add amoo 
	virtual void AddAmmo(float newAmmo);

	// Can Reload
	bool CanReload();

	// Can Fire
	bool CanFire();

};


// FIteData Struct
USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_USTRUCT_BODY()
public:

	// Item Name
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
		FString ItemName;

	//Item Icon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
		UTexture2D* ItemIcon;

	// Item Count
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
		int32 ItemCount;

	// Ite Weight
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
		float Weight;

	// Item Archtype/Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
		TSubclassOf<class AItem> Archetype;

	// If Weapon , main fire data
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
		FWeaponData TheWeaponData;


	FItemData(){};
	FItemData(TSubclassOf<class AItem> Item, FString newItemName = FString(""), UTexture2D* newItemIcon = NULL, float newWeight = 0.1, int32 newItemCount = 1);

	// Set Item Data
	void SetItemData(FItemData newData);
};



UCLASS()
class RAGE_API URageData : public UObject
{
	GENERATED_BODY()
};
