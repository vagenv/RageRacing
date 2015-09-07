// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RageBaseCar.h"
#include "RagePlayerCar.generated.h"

/**
 *		Base for All Player Cars
 */
UCLASS()
class RAGE_API ARagePlayerCar : public ARageBaseCar
{
	GENERATED_BODY()
	

	/** Spring arm that will offset the camera */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* SpringArm;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* Camera;

	/** Camera component for the In-Car view */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* InternalCamera;




public:

	ARagePlayerCar();
		/*

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
		UPROPERTY(VisibleDefaultsOnly, Category = Mesh)

		UFUNCTION(BlueprintImplementableEvent, Category = " ")

		UFUNCTION(BlueprintCallable, Category = " ")

		UFUNCTION(BlueprintCallable, BlueprintPure, Category = " ")

		*/


	virtual void BeginPlay() override;


	void EquipDefaultWeapons();

	FTimerHandle BoostDelayTimerHandle;


	/** The current speed as a string eg 10 km/h */
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
		FText SpeedDisplayString;

	/** The current gear as a string (R,N, 1,2 etc) */
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
		FText GearDisplayString;

	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
		/** The color of the incar gear text in forward gears */
		FColor	GearDisplayColor;

	/** The color of the incar gear text when in reverse */
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
		FColor	GearDisplayReverseColor;

	/** Are we using incar camera */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly)
		bool bInCarCameraActive;

	/** Are we in reverse gear */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly)
		bool bInReverseGear;

	/** Initial offset of incar camera */
	FVector InternalCameraOrigin;

	// Begin Pawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End Pawn interface



	/** Handle pressing forwards */
	virtual void MoveForward(float Val);




	/** Handle pressing right */
	virtual void MoveRight(float Val);
	/** Handle handbrake pressed */
	void OnHandbrakePressed();
	/** Handle handbrake released */
	void OnHandbrakeReleased();
	/** Switch between cameras */
	virtual void OnToggleCamera();

	static const FName LookUpBinding;
	static const FName LookRightBinding;


	/**
	* Activate In-Car camera. Enable camera and sets visibility of incar hud display
	*
	* @param	bState true will enable in car view and set visibility of various
	*/
	virtual void EnableIncarView(const bool bState);




	/** Returns SpringArm subobject **/
	FORCEINLINE virtual USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE virtual UCameraComponent* GetCamera() const { return Camera; }
	/** Returns InternalCamera subobject **/
	FORCEINLINE virtual UCameraComponent* GetInternalCamera() const { return InternalCamera; }




	// Inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		TArray<class AItem*> ItemList;


	////////					  Weapon Part

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<class AWeapon> DefaultMainWeapon;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<class AWeapon> DefaultAltWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class AWeapon* MainWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class AWeapon* AltWeapon;



	// Inventory

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
		void BP_ItemPickedUp(class AItem* TheItem, TSubclassOf<class AItem>  TheSubClass);

	void ItemPickup(TSubclassOf<class AItem>  TheItem);


	void InventoryUpdated();
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
		void BP_InventoryUpdated();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "")
	TArray< class AItem* > GetTheItemList();






	class ARageHUD* TheHUD;
	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	TArray<FWeaponSlot> WeaponMainList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	TArray<FWeaponSlot> WeaponAltList;
	*/



	//////					 Energy Part


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
		float Energy_MaxValue = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
		float Energy_CurrentValue = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
		float Energy_RestoreSpeed = 0.1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
		float Energy_RestoreValue = 1;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
		float Energy_BoostMultiplier = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
		float Energy_BoostMinValue = 40;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
		float Energy_JumpMultiplier = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
		FVector2D Energy_JumpDirection =FVector2D(0.5,1) ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
		float Energy_BoostJumpSelectDelay=0.3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
		float Energy_BoostJumpMinValue= 50;




	UFUNCTION(BlueprintCallable, Category = "Energy")
		void UseAllEnergy();
	UFUNCTION(BlueprintCallable, Category = "Energy")
		void UseEnergy(float Value);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Energy")
		bool CanBoost();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Energy")
		bool CanJump();


	UFUNCTION(BlueprintCallable, Category = "Energy")
		void StartEneryRestore();
	UFUNCTION(BlueprintCallable, Category = "Energy")
		void StopEneryRestore();

	FTimerHandle EnergyRestoreHandle;

	void Energy_Restore();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TheCar")
		float GetEnergyPercent();


	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void EquipNewWeapon(class AWeapon* TheWeapon);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void UnequipWeapon(uint8 WeaponType);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void SearchNewWeaponEquip();


	// Bind External Calling

	void ActionDown();
	void ActionUp();

	void AltActionDown();
	void AltActionUp();

	void Boost();
	void BoostDown();
	void BoostUp();


	void NextAction();
	void NextAltAction();
	

	void DoubleJump();

protected:

	// Callabale action Events
	UFUNCTION(BlueprintImplementableEvent, Category = " Main Events ")
		void BP_ActionDown();
	UFUNCTION(BlueprintImplementableEvent, Category = " Main Events ")
		void BP_ActionUp();

	UFUNCTION(BlueprintImplementableEvent, Category = " Main Events ")
		void BP_NextAction();
	UFUNCTION(BlueprintImplementableEvent, Category = " Main Events ")
		void BP_NextAltAction();


	UFUNCTION(BlueprintImplementableEvent, Category = " Main Events ")
		void BP_AltActionDown();
	UFUNCTION(BlueprintImplementableEvent, Category = " Main Events ")
		void BP_AltActionUp();

	UFUNCTION(BlueprintImplementableEvent, Category = " Main Events ")
		void BP_BoostDown();
	UFUNCTION(BlueprintImplementableEvent, Category = " Main Events ")
		void BP_Boost();
	UFUNCTION(BlueprintImplementableEvent, Category = " Main Events ")
		void BP_BoostUp();

	UFUNCTION(BlueprintImplementableEvent, Category = " Main Events ")
		void BP_DoubleJump();

	

	/*
	
	UPROPERTY(EditAnywhere, Category = "Energy")
		FRuntimeFloatCurve Energy_BoostCurve;

	
	*/
};
