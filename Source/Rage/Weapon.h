// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item.h"
#include "Weapon.generated.h"


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

	UPROPERTY(Category = "Mesh", EditAnywhere, BlueprintReadOnly)
		FName AttachSocket;


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
		bool DrawDebugData=false;


	
protected:
	virtual void PreFire();
	virtual void Fire(){ BP_Fire(); };
private:

	FTimerHandle FireHandle;
	bool CanShoot();
	bool bShooting;
	bool bFirePressed;


};
