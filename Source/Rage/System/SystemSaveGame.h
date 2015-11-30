// Copyright 2015 Vagen Ayrapetyan

#pragma once

#include "GameFramework/SaveGame.h"
#include "Inventory/Inventory.h"
#include "RageData.h"
#include "SystemSaveGame.generated.h"


UCLASS()
class RAGE_API USystemSaveGame : public USaveGame
{
	GENERATED_BODY()


public:
	USystemSaveGame(const class FObjectInitializer& PCIP);

	// List of Player Inventory Items
	UPROPERTY(VisibleAnywhere, Category = Basic)
		TArray<FItemData> Items;


	// Player Name
	UPROPERTY(VisibleAnywhere, Category = Basic)
		FString PlayerName;

	// Player Color
	UPROPERTY(VisibleAnywhere, Category = Basic)
		FPlayerSaveData PlayerData;

	// Save Slot Name
	UPROPERTY(VisibleAnywhere, Category = Basic)
		FString SaveSlotName;
	// User Index
	UPROPERTY(VisibleAnywhere, Category = Basic)
		uint32 UserIndex;
	
};
