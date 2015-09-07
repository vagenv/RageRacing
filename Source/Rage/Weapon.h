// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item.h"
#include "Weapon.generated.h"


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


UCLASS()
class RAGE_API AWeapon : public AItem
{
	GENERATED_BODY()

	

public:
	AWeapon(const class FObjectInitializer& PCIP);
	virtual void BeginPlay() override;


	class ARageBaseCar* TheCar;

	void FireStart();
	void FireEnd();

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon Event")
		void BP_Fire();


	UPROPERTY(Category = "Mesh", VisibleDefaultsOnly, BlueprintReadOnly)
		UStaticMeshComponent* TheStaticMeshComponent;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		EWeaponArchetype WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		TArray<FName> AttachSockets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		FName AttachSocket="";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float ClipSize=10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float CurrentAmmo=5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float FireDamage=10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float FireSpeed=1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float FireCost=1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float FireDistance=1200;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float WeaponFireFeedback = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		bool bRestoreAmmo = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float AmmoRestoreSpeed=0.0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float AmmoRestoreValue =0.0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		EAmmoDisplayType AmmoDisplayType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		bool DrawDebugData=false;


	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon")
		float GetCurrentAmmoPercent();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void AddAmmoValue(float AmmoAmount);
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void AddAmmoPickup(AWeapon* WeaponPickup);


	
protected:
	virtual void PreFire();
	virtual void Fire(){ BP_Fire(); };
private:

	FTimerHandle FireHandle;
	bool CanShoot();
	bool bShooting;
	bool bFirePressed;


};
