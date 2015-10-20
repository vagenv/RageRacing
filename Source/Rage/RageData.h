// Copyright 2015 Vagen Ayrapetyan

#pragma once

#include "Object.h"
#include "RageData.generated.h"


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EWeaponArchetype : uint8
{
	Main						UMETA(DisplayName = "Main Weapon"),
	Secondary					UMETA(DisplayName = "Secondary Weapon"),
	Ultimate					UMETA(DisplayName = "Ultimate Weapon"),

};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EAmmoDisplayType : uint8
{
	Digit						UMETA(DisplayName = "Numberical"),
	Bar							UMETA(DisplayName = "Bar"),
};




USTRUCT(BlueprintType)
struct FBasicWeaponData
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
		class AWeapon* WeaponArchype;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
		bool bUseAmmo = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
		float ClipSize = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
		float CurrentAmmo = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
		float FireDamage = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
		float FireSpeed = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
		float FireCost = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
		float FireDistance;

	//virtual void AddAmmo(float newAmmo, int32 newClip);


};

UCLASS()
class RAGE_API URageData : public UObject
{
	GENERATED_BODY()
};
