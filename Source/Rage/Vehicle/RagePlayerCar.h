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
	
		/*

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
		UPROPERTY(VisibleDefaultsOnly, Category = "")
		UFUNCTION(BlueprintImplementableEvent, Category = " ")
		UFUNCTION(BlueprintCallable, Category = " ")
		UFUNCTION(BlueprintCallable, BlueprintPure, Category = " ")
	
		UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable, Category = "Input")
		void ();
		bool _Validate();
		void _Implementation();
		*/


public:

	// Constructor
	ARagePlayerCar();

	// Begin Play
	virtual void BeginPlay() override;

	// Post Begin Play
	virtual void PostBeginPlay()override;

	// Tick/Update
	virtual void Tick(float Delta)override;


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//									Main Player Components


	// Spring arm that will offset the camera 
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* SpringArm;

	// Camera component that will be our viewpoint 
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* Camera;

	// Camera component for the In-Car view 
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* InternalCamera;


	// Energy Data
	UPROPERTY(ReplicatedUsing = OnRep_BoostEnergyReplicated, EditAnywhere, BlueprintReadWrite, Category = "Energy")
		FBoostEnergyData TheBoostEnergyData;

	UPROPERTY(ReplicatedUsing = OnRep_ShieldEnergyReplicated, EditAnywhere, BlueprintReadWrite, Category = "Shield")
		FShieldEnergyData TheShieldEnergyData;
	

	// The Controller
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "GamePlay")
	class APlayerController * ThePC;

	// The Inventory
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	class UInventory * TheInventory;

	// The HUD
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	class ARageHUD* TheHUD;

	// Current Item Pickup Selected
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	class AItemPickup * currentPickup;



	// Current Item Pickup Selected
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
		class AWeapon * PendingWeaponEquip;


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//								Components Functions and Events




	////////				 Camera


	// Called to wwitch between cameras 
	UFUNCTION(BlueprintCallable, Category = "Camera")
		virtual void OnToggleCamera();

	// Are we using Third Person Camera?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		bool bThirdPersonCamera = true;

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	///						Boost Energy

	// Use All Boost Energy
	UFUNCTION(BlueprintCallable, Category = "Energy")
		void UseAllBoostEnergy();

	// Use Some Boost Energy
	UFUNCTION(BlueprintCallable, Category = "Energy")
		void UseBoostEnergy(float Value);


	// Can Boost
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Energy")
		bool CanBoost();

	// Can Boost Jump
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Energy")
		bool CanJump();


	// Enable Boost Energy Restore
	UFUNCTION(BlueprintCallable, Category = "Energy")
		void StartBoostEnergyRestore();

	// Disable Boost Energy Restore
	UFUNCTION(BlueprintCallable, Category = "Energy")
		void StopBoostEnergyRestore();


	// Get  Boost Energy Percent
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Energy")
		float GetBoostEnergyPercent();


	UFUNCTION()
		void OnRep_BoostEnergyReplicated();


private:
	// Energy Restore Timer Handle
	FTimerHandle BoostEnergyRestoreHandle;

	// Energy Restore Event
	void BoostEnergy_Restore();
public:

	///////////////////////////////////////////////////////////////////////////////////////////////////////////


	///						Shield

	// Use Some Shield Energy
	UFUNCTION(BlueprintCallable, Category = "Shield")
		void UseShieldEnergy(float Value);

	// Enable Shield Energy Restore
	UFUNCTION(BlueprintCallable, Category = "Shield")
		void StartShieldEnergyRestore();

	// Disable Shield Energy Restore
	UFUNCTION(BlueprintCallable, Category = "Shield")
		void StopShieldEnergyRestore();


	// Get Shield Energy Percent
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Shield")
		float GetShieldEnergyPercent();


	// When Item Is Used
	UFUNCTION(BlueprintImplementableEvent, Category = "Shield")
		void BP_ShieldEnergyValueUpdated();

	UFUNCTION()
		void OnRep_ShieldEnergyReplicated();


private:
	// Energy Restore Timer Handle
	FTimerHandle ShieldEnergyRestoreHandle;

	// Delayed Energy Start
	FTimerHandle ShieldEnergyRestoreStartDelayHandle;

	// Energy Restore Event
	void ShieldEnergy_Restore();
public:



	///////////////////////////////////////////////////////////////////////////////////////////////////////////



	//					Weapon
	// Equip new Weapon
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void EquipNewWeapon(class AWeapon* TheWeapon);

	// Unequip Weapon
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void UnEquipWeapon(EWeaponArchetype WeaponType);


	// Current Item Pickup Selected
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float WeaponSwitchDelay = 0.1f;


	// Search new Weapon
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void SearchNewWeaponEquip();

	// One of Weapons ammo finished
	void WeaponAmmoFinished();

	// Called on Client when One of the weapons was updated
	virtual void OnRep_WeaponsUpdated()override;




	// Check player state if he can fire
	virtual bool CanShoot();


	///////////////////////////////////////////////////////////////////////////////////////////////////////////


	////				Inventory

	// Inventory List Updated
	void InventoryUpdated();

	// Blueprint Event that Inventory Updated
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
		void BP_InventoryUpdated();

	// Pickup currently highlighted item
	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void PickupTheItemPickup();



	// Use Item Called On Server
	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable, Category = "Inventory")
		void Server_UseItem(AItem* TheItem);
	bool Server_UseItem_Validate(AItem* TheItem);
	void Server_UseItem_Implementation(AItem* TheItem);


	// When Item Is Used
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
		void BP_ItemUsed(class AItem* TheItem);

	// When Item is dropped
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
		void BP_ItemDroped(class AItemPickup* thePickup);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////


	//								CarReset 


	// Is the car turned over
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State")
		bool IsTurnedOver();

	// Turned Over Valid Angle
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "State")
		float TurnedOverValidAngle = 130;
	// Turned Over Max Velocity
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "State")
		float TurnedOverValidVelocity = 300;

	// Restore Height offset
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "State")
		float ResetRestoreHeight = 100;


	///////////////////////////////////////////////////////////////////////////////////////////////////////////


	///				Death/Revive/ DMG

	// Player Died
	virtual void Death()override;

	// Player Revived
	virtual void Revive()override;

	// Take Damage override
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)override;

	// Take Damage override
	virtual void ShieldAbsorbDamage(float& DamageAmount, struct FDamageEvent const& DamageEvent);


private:
		// Called after some delay between switching weapons
		void EquipPendingWeapon();

		// Equip Default Weapons
		virtual void EquipDefaultWeapons()override;
public:





	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//							Additonal Properties and Events


	// Reset the car function
	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable, Category = "Network")
		void AddChatMessage(const FString & TheMessage);
	bool AddChatMessage_Validate(const FString & TheMessage);
	void AddChatMessage_Implementation(const FString & TheMessage);



	// Reset the car function
	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable, Category = "Status")
		void SetPlayerStats(const FString & newPlayerName, FLinearColor newPlayerColor, FVector newAdditonalColorProperties);
	bool SetPlayerStats_Validate(const FString & newPlayerName, FLinearColor newPlayerColor, FVector newAdditonalColorProperties);
	void SetPlayerStats_Implementation(const FString & newPlayerName, FLinearColor newPlayerColor, FVector newAdditonalColorProperties);








	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//								Input Part


private:

	// Internal ->   Boost
	void Boost();

	// Internal ->   Boost Jump
	void DoubleJump();

	// Internal ->   Fire Start
	void FireStart();
	// Internal ->   Fire End
	void FireEnd();

	// Internal ->   Alt Fire Start
	void AltFireStart();
	// Internal ->   Alt Fire End
	void AltFireEnd();

	// Internal ->   Bind tilt to internal event
	virtual void AddGravityMotionInput(FVector Val);

	// Bind Input to internal functions
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	// --- Landsaped/Inverted Landsace  ->  Between 1 and -1
	float ANDROID_Tilt_OrientationValue = 1;
public:


	//  Android Vertical Tilt value Curve
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		FRuntimeFloatCurve VerticalInputCurve;

	//  Android Horizontal Tilt value Curve
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		FRuntimeFloatCurve HorizontalInputCurve;


	// Vertical Move  
	UFUNCTION(BlueprintCallable, Category = "Input")
		virtual void AddVerticalInput(float Val);

	//Horizontal Move
	UFUNCTION(BlueprintCallable, Category = "Input")
		virtual void AddHorizontalInput(float Val);

	// Blueprint Boost
	UFUNCTION(BlueprintImplementableEvent, Category = "State")
		void BP_Boost();

	// Blueprint Double Jump
	UFUNCTION(BlueprintImplementableEvent, Category = "State")
		void BP_DoubleJump();



	// Reset the car function
	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable, Category = "State")
		void ResetCar();
	bool ResetCar_Validate();
	void ResetCar_Implementation();


	// BP event when car was reseted
	UFUNCTION(BlueprintImplementableEvent, Category = "State")
		void BP_CarWasReseted();



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//								Add Input from client


	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable, Category = "Input")
		void AddInput_FireStart();
		bool AddInput_FireStart_Validate();
		void AddInput_FireStart_Implementation();


	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable, Category = "Input")
		void AddInput_FireEnd();
		bool AddInput_FireEnd_Validate();
		void AddInput_FireEnd_Implementation();

	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable, Category = "Input")
		void AddInput_AltFireStart();
		bool AddInput_AltFireStart_Validate();
		void AddInput_AltFireStart_Implementation();



	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable, Category = "Input")
		void AddInput_AltFireEnd();
		bool AddInput_AltFireEnd_Validate();
		void AddInput_AltFireEnd_Implementation();

	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable, Category = "Input")
		void AddInput_Boost();
		bool AddInput_Boost_Validate();
		void AddInput_Boost_Implementation();

	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable, Category = "Input")
		void AddInput_JumpBoost();
		bool AddInput_JumpBoost_Validate();
		void AddInput_JumpBoost_Implementation();



	// Get Acces to internal Components

	// Returns SpringArm subobject 
	FORCEINLINE virtual USpringArmComponent* GetSpringArm() const { return SpringArm; }

	// Returns Camera subobject 
	FORCEINLINE virtual UCameraComponent* GetCamera() const { return Camera; }

	// Returns InternalCamera subobject 
	FORCEINLINE virtual UCameraComponent* GetInternalCamera() const { return InternalCamera; }


};
