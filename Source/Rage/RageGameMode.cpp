// Copyright 2015 Vagen Ayrapetyan


#include "Rage.h"
#include "RageGameMode.h"
/*
#include "Vehicle/RageBaseCar.h"
#include "RageHUD.h"
*/
#include "Custom/SystemSaveGame.h"

ARageGameMode::ARageGameMode()
{
	//DefaultPawnClass = ARageBaseCar::StaticClass();
	//HUDClass = ARageHUD::StaticClass();
}



void ARageGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Start Post begin delay
	FTimerHandle MyHandle;
	GetWorldTimerManager().SetTimer(MyHandle, this, &ARageGameMode::PostBeginPlay, 0.1f, false);

}

// Post begin Play
void ARageGameMode::PostBeginPlay()
{
	if (Role < ROLE_Authority)return;


	// Loading Save file
	USystemSaveGame* LoadGameInstance = Cast<USystemSaveGame>(UGameplayStatics::CreateSaveGameObject(USystemSaveGame::StaticClass()));
	if (LoadGameInstance)
	{
		LoadGameInstance = Cast<USystemSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->UserIndex));
		if (LoadGameInstance)
		{
			// Save File Found
			SaveFile = LoadGameInstance;
		}
	}
}

// End Game
void ARageGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	if (Role >= ROLE_Authority)
	{

		// No Save File, Creating new
		if (!SaveFile)
		{
			USystemSaveGame* SaveFile = Cast<USystemSaveGame>(UGameplayStatics::CreateSaveGameObject(USystemSaveGame::StaticClass()));
			UGameplayStatics::SaveGameToSlot(SaveFile, SaveFile->SaveSlotName, SaveFile->UserIndex);
		}
		if (SaveFile)
		{
			// Saving level block
			// Saving Save File
			UGameplayStatics::SaveGameToSlot(SaveFile, SaveFile->SaveSlotName, SaveFile->UserIndex);
		}
	}
	Super::EndPlay(EndPlayReason);
}
