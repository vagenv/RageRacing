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


	FTimerHandle BoostTimerHandle;


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





	////////					  Weapon Part

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<class AWeapon> DefaultMainWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class AWeapon* MainWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<class AWeapon> DefaultAltWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class AWeapon* AltWeapon;
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
		float Energy_JumpMultiplier = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
		FVector2D Energy_JumpDirection =FVector2D(0.5,1) ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
		float Energy_BoostJumpSelectDelay=0.3;


	void Energy_Restore();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TheCar")
		float GetEnergyPercent();



	// Bind External Calling

	void ActionDown();
	void ActionUp();

	void AltActionDown();
	void AltActionUp();

	void Boost();
	void BoostDown();
	void BoostUp();
	

	void DoubleJump();

protected:

	// Callabale action Events
	UFUNCTION(BlueprintImplementableEvent, Category = " Main Events ")
		void BP_ActionDown();
	UFUNCTION(BlueprintImplementableEvent, Category = " Main Events ")
		void BP_ActionUp();



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

	
};
