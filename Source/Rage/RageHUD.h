// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/HUD.h"
#include "RageHud.generated.h"


UCLASS(config = Game)
class ARageHUD : public AHUD
{
	GENERATED_BODY()

public:
	ARageHUD();

	void BeginPlay()override;

	void PostBeginPlay();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
		float PostDelay=0.3;


	UFUNCTION(BlueprintImplementableEvent, Category = "")
		void BP_InventoryUpdated();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "")
	class ARagePlayerCar* GetCar();

};
