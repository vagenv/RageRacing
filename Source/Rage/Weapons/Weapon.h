// Copyright 2015 Vagen Ayrapetyan

#pragma once

#include "Inventory/Item.h"
#include "Weapon.generated.h"


UCLASS()
class RAGE_API AWeapon : public AItem
{
	GENERATED_BODY()


public:


	AWeapon(const class FObjectInitializer& PCIP);

	virtual void BeginPlay() override;




	//				 System Components
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		EWeaponArchetype WeaponType;


	UPROPERTY(Category = "Mesh", VisibleDefaultsOnly, BlueprintReadOnly)
		UStaticMeshComponent* TheStaticMeshComponent;




	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		FName AttachSocketName = " ";


	//      Fire Props

	// Use Ammo on Fire
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		bool bUseAmmo = true;

	// Main Fire Data
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		FWeaponData WeaponData;

	// Weapon Equip Time
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float EquipTime = 0.25;

	// Weapon Reload Time
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float ReloadTime = 0.6;


	// Is Weapon Shooting
	bool bShooting = false;

	// Is Reloading
	bool bReloading = false;

	// Is Weapon Equiped
	bool bEquiped = false;

private:

	// Main Fire Timer Handle
	FTimerHandle PreFireTimeHandle;



public:

	// The Actual Fire Event
	virtual void Fire(){ BP_Fire(); }

	// Check Everything before the actual fire Example: check ammo, check player state, use ammo , etc
	virtual void PreFire();

	// Player Pressed Fire
	virtual void FireStart();

	// Player Released Fire
	virtual void FireEnd();


	// Save the current/equiped weapon stats to inventory
	void SaveCurrentWeaponStats();


	// Equip Started
	virtual void EquipStart();

	// Equip Ended
	virtual void EquipEnd();

	// Unequip Started
	virtual void UnEquipStart();

	// Unequip ended
	virtual void UnEquipEnd();


	// Can Player Fire
	virtual bool CanShoot();

	// Use Main Fire Ammo
	void UseAmmo();


	void AttachWeapon();



	// Add Ammo by numbers 
	virtual void AddAmmo(float newAmmo);

	// Add Ammo from weapon
	virtual void AddAmmo(AWeapon* weapAmmo);



	virtual void InventoryUse(class ARagePlayerCar* Player)override;


	//				Blueprint Events 


	// Hit Enemy Character
	UFUNCTION(BlueprintImplementableEvent, Category = "Fire")
		void BP_HitEnemy(FHitResult HitData);

	// Blueprint Event:  Pre Fire
	UFUNCTION(BlueprintImplementableEvent, Category = "Fire")
		void BP_PreFire();

	// Blueprint Event:  Fire
	UFUNCTION(BlueprintImplementableEvent, Category = "Fire")
		void BP_Fire();

	// Blueprint Event:  Fire Pressed
	UFUNCTION(BlueprintImplementableEvent, Category = "Fire")
		void BP_FirePress_Start();

	// Blueprint Event:  Fire  Released
	UFUNCTION(BlueprintImplementableEvent, Category = "Fire")
		void BP_FirePress_End();

	// Blueprint Event:  Tried firing , but no ammo
	UFUNCTION(BlueprintImplementableEvent, Category = "Fire")
		void BP_NoAmmo();

	// Blueprint Event:   Equip Start
	UFUNCTION(BlueprintImplementableEvent, Category = "Fire")
		void BP_Equip_Start();

	// Blueprint Event:   Equip End
	UFUNCTION(BlueprintImplementableEvent, Category = "Fire")
		void BP_Equip_End();

	// Blueprint Event:   UnEquip Start
	UFUNCTION(BlueprintImplementableEvent, Category = "Fire")
		void BP_Unequip_Start();

	// Blueprint Event:   unEquip End
	UFUNCTION(BlueprintImplementableEvent, Category = "Fire")
		void BP_Unequip_End();

	// Blueprint Event: Main Fire Ammo Used  
	UFUNCTION(BlueprintImplementableEvent, Category = "Fire")
		void BP_Ammo_Used();

};
