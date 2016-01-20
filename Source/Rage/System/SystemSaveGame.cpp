// Copyright 2015-2016 Vagen Ayrapetyan

#include "Rage.h"
#include "System/SystemSaveGame.h"


USystemSaveGame::USystemSaveGame(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	SaveSlotName = TEXT("RadeSaveSlot");
	UserIndex = 0;
}

