// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/WheeledVehicle.h"
#include "RageBaseCar.generated.h"

class UPhysicalMaterial;
class UCameraComponent;
class USpringArmComponent;
class UTextRenderComponent;
class UInputComponent;
class UActionComponent;

UCLASS(config=Game)
class ARageBaseCar : public AWheeledVehicle
{
	GENERATED_BODY()

	/// Main Components

	/** Spring arm that will offset the camera */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	/** Camera component for the In-Car view */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* InternalCamera;


	/** Audio component for the engine sound */
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* EngineSoundComponent;

	/** Audio component for the engine sound */
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UActionComponent* TheActionComponent;


public:
	ARageBaseCar();

	virtual void InputAction();
	virtual void InputAltAction();
	virtual void InputBoost();


	/*

	UFUNCTION(BlueprintImplementableEvent, Category = " ")

	UFUNCTION(BlueprintCallable, Category = " ")

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = " ")
	
	*/


	///////////                                                                        Legacy part

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

	// Begin Actor interface
	virtual void Tick(float Delta) override;
	virtual void BeginPlay() override;
	// End Actor interface

	/** Handle pressing forwards */
	virtual void MoveForward(float Val);



	/** Update the physics material used by the vehicle mesh */
	virtual void UpdatePhysicsMaterial();

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
	static const FName EngineAudioRPM;


	/**
	* Activate In-Car camera. Enable camera and sets visibility of incar hud display
	*
	* @param	bState true will enable in car view and set visibility of various
	*/
	virtual void EnableIncarView(const bool bState);



	/* Are we on a 'slippery' surface */
	bool bIsLowFriction;
	/** Slippery Material instance */
	UPhysicalMaterial* SlipperyMaterial;
	/** Non Slippery Material instance */
	UPhysicalMaterial* NonSlipperyMaterial;


public:
	/** Returns SpringArm subobject **/
	FORCEINLINE virtual USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE virtual UCameraComponent* GetCamera() const { return Camera; }
	/** Returns InternalCamera subobject **/
	FORCEINLINE virtual UCameraComponent* GetInternalCamera() const { return InternalCamera; }
	/** Returns EngineSoundComponent subobject **/
	FORCEINLINE virtual UAudioComponent* GetEngineSoundComponent() const { return EngineSoundComponent; }

	FORCEINLINE virtual UActionComponent* GetActionComponent() const { return TheActionComponent; }


};
