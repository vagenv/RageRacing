// Copyright 2015 Vagen Ayrapetyan

#pragma once

#include "Vehicle/RageBaseCar.h"
#include "RageData.h"
#include "RagePlayerCar.generated.h"

/**
 *		Base for All Player Cars
 */
UCLASS()
class RAGE_API ARagePlayerCar : public ARageBaseCar
{
	GENERATED_BODY()
	

	// Spring arm that will offset the camera 
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* SpringArm;

	// Camera component that will be our viewpoint 
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* Camera;

	// Camera component for the In-Car view 
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* InternalCamera;


public:


	/*

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
	UPROPERTY(VisibleDefaultsOnly, Category = "")
	UFUNCTION(BlueprintImplementableEvent, Category = " ")
	UFUNCTION(BlueprintCallable, Category = " ")
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = " ")
	*/


	ARagePlayerCar();

	virtual void BeginPlay() override;


private:
	// Equip Default Weapons
	virtual void EquipDefaultWeapons()override;


	// Boost Delay Timer
	FTimerHandle BoostDelayTimerHandle;


	// Setup Input
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	


	// Vertical Move  
	virtual void MoveForward(float Val);

	//Horizontal Move
	virtual void MoveRight(float Val);

public:
	// Switch between cameras 

	UFUNCTION(BlueprintCallable, Category = "Camera")
	virtual void OnToggleCamera();

	// Are we using incar camera 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		bool bThirdPersonCamera = true;

	// The Controller
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	class APlayerController * ThePC;

	// The Inventory
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	class AInventory * TheInventory;
	// The HUD
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
	class ARageHUD* TheHUD;

	// Current Item Pickup Selected
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	class AItemPickup * currentPickup;



	// Current Item Pickup Selected
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		class AWeapon * PendingWeaponEquip;
	// Current Item Pickup Selected
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float WeaponSwitchDelay=0.1f;
private :
	void EquipPendingWeapon();
public:






	// Inventory List Updated
	void InventoryUpdated();
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
		void BP_InventoryUpdated();


	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void PickupTheItemPickup();
	



	// Energy Data
	UPROPERTY(Replicated,EditAnywhere, BlueprintReadWrite, Category = "Energy")
		FEnergyData TheEnergyData;


	// Use All Energy
	UFUNCTION(BlueprintCallable, Category = "Energy")
		void UseAllEnergy();

	// Use Some Energy
	UFUNCTION(BlueprintCallable, Category = "Energy")
		void UseEnergy(float Value);




	// Check player state if he can fire
	virtual bool CanShoot();

	// Can Boost
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Energy")
		bool CanBoost();

	// Can Boost Jump
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Energy")
		bool CanJump();



	// Is the car turned over
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TheCar")
		bool IsTurnedOver();
	// Turned Over Max Angle
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "TheCar")
		float TurnedOverValidAngle = 150;
	// Turned Over Max Angle
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "TheCar")
		float TurnedOverValidVelocity = 200;

	// Reset the car function
	UFUNCTION(BlueprintCallable, Category = "TheCar")
		void ResetCar();
	// BP event when car was reseted
	UFUNCTION(BlueprintImplementableEvent, Category = "TheCar")
		void BP_CarWasReseted();

	// Enable Energy Restore
	UFUNCTION(BlueprintCallable, Category = "Energy")
		void StartEneryRestore();

	// Disable Energy Restore
	UFUNCTION(BlueprintCallable, Category = "Energy")
		void StopEneryRestore();

private:
	// Energy Restore Timer Handle
	FTimerHandle EnergyRestoreHandle;

	// Energy Restore Event
	void Energy_Restore();
public:
	// Get Energy Percent
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TheCar")
		float GetEnergyPercent();


	// Equip new Weapon
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void EquipNewWeapon(class AWeapon* TheWeapon);

	// Unequip Weapon
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void UnEquipWeapon(EWeaponArchetype WeaponType);

	// Search new Weapon
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void SearchNewWeaponEquip();


	// Bind External Calling
private:
	void Boost();

	void DoubleJump();

	void FireStart();
	void FireEnd();


	void AltFireStart();
	void AltFireEnd();

public:


	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void AddInput_FireStart();
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void AddInput_FireEnd();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void AddInput_AltFireStart();
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void AddInput_AltFireEnd();




	UFUNCTION(BlueprintImplementableEvent, Category = "Boost")
		void BP_Boost();

	UFUNCTION(BlueprintImplementableEvent, Category = "Boost")
		void BP_DoubleJump();


	// When Item Is Used
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
		void BP_ItemUsed(class AItem* TheItem);

	// When Item is dropped
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
		void BP_ItemDroped(class AItemPickup* thePickup);



	// Returns SpringArm subobject 
	FORCEINLINE virtual USpringArmComponent* GetSpringArm() const { return SpringArm; }

	// Returns Camera subobject 
	FORCEINLINE virtual UCameraComponent* GetCamera() const { return Camera; }

	// Returns InternalCamera subobject 
	FORCEINLINE virtual UCameraComponent* GetInternalCamera() const { return InternalCamera; }


};
