// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Rage.h"
#include "RageGameMode.h"
#include "RageBaseCar.h"
#include "RageHud.h"

ARageGameMode::ARageGameMode()
{
	DefaultPawnClass = ARageBaseCar::StaticClass();
	HUDClass = ARageHUD::StaticClass();
}
