// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RageBaseCar.h"
#include "RageDirtCar.generated.h"

/**
 * 
 */
UCLASS()
class RAGE_API ARageDirtCar : public ARageBaseCar
{
	GENERATED_BODY()
public:
	ARageDirtCar();

	void BeginPlay()override;

	void Tick(float deltaTime)override;

	/** Text component for the In-Car speed */
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* InCarSpeed;

	/** Text component for the In-Car gear */
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* InCarGear;


	/** Returns InCarSpeed subobject **/
	FORCEINLINE UTextRenderComponent* GetInCarSpeed() const { return InCarSpeed; }
	/** Returns InCarGear subobject **/
	FORCEINLINE UTextRenderComponent* GetInCarGear() const { return InCarGear; }

	/** Setup the strings used on the hud */
	virtual void SetupInCarHUD();

	void EnableIncarView(const bool bState) override;
	

	/** Update the gear and speed strings */
	virtual void UpdateHUDStrings();
};
