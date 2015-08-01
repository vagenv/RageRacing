// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Rage.h"
#include "RageGameMode.h"
#include "RagePawn.h"
#include "RageHud.h"

ARageGameMode::ARageGameMode()
{
	DefaultPawnClass = ARagePawn::StaticClass();
	HUDClass = ARageHud::StaticClass();
}
