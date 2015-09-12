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




USTRUCT(BlueprintType)
struct FBasicWeaponData
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
		AWeapon* WeaponArchype;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
		bool bUseAmmo=true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
		float ClipSize=10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
		float CurrentAmmo=5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
		float FireDamage=1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
		float FireSpeed=1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
		float FireCost=1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
		float FireDistance;

	//virtual void AddAmmo(float newAmmo, int32 newClip);
	

};



UCLASS()
class RAGE_API AWeapon : public AItem
{
	GENERATED_BODY()

	
			/*
		UFUNCTION(Reliable, Server, WithValidation)
	
		bool ADarkPlayer::ServerSetAnimID_Validate(uint8 AnimID){
			return true;
		}
		void ADarkPlayer::ServerSetAnimID_Implementation(uint8 AnimID){
			GlobalSetAnimID(AnimID);
		}
		, NetMulticast, Reliable,
	*/


public:
	AWeapon(const class FObjectInitializer& PCIP);


	//					Basic Events

	virtual void BeginPlay() override;


	//				 System Components


	UPROPERTY(Category = "Mesh", VisibleDefaultsOnly, BlueprintReadOnly)
		UStaticMeshComponent* TheStaticMeshComponent;




	//				 System References

	class ARageBaseCar* TheCar;


	//				 Internal Events

	UFUNCTION(Reliable, Server, WithValidation)
		void FireStart();
	UFUNCTION(Reliable, Server, WithValidation)
		void FireEnd();

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon Event")
		void BP_Fire();
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon Event")
		void BP_Server_Fire();

	virtual void PreFire();
	virtual void Fire(){ Global_Fire(); BP_Server_Fire(); };

	/*
	UFUNCTION(Reliable, Server, WithValidation)
		void Server_Fire();
		*/
	UFUNCTION(Reliable, NetMulticast, WithValidation)
		void Global_Fire();







	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon")
		float GetCurrentAmmoPercent();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void AddAmmoValue(float AmmoAmount);
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void AddAmmoPickup(AWeapon* WeaponPickup);
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void UseAmmo();

	bool HasAmmo();



private:

	FTimerHandle FireHandle;
	bool CanShoot();
	bool bShooting;
	bool bFirePressed;

	//				 Weapon Properties 

	/////////////////////////////////////////////////////////////////////////////////
public:


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		EWeaponArchetype WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		TArray<FName> AttachSockets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		FName AttachSocket="";


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		FBasicWeaponData WeaponData;

	/*
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
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		EAmmoDisplayType AmmoDisplayType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		bool DrawDebugData=false;



	



};
