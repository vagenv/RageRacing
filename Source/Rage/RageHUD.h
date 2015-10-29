// Copyright 2015 Vagen Ayrapetyan

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
	class ARagePlayerCar* GetPlayer();


	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item")
	class AItem* ItemClassToItemRef(TSubclassOf<class AItem> TheItemClass)const;

};
