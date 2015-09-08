// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Rage.h"
#include "RageHUD.h"
#include "Vehicle/RagePlayerCar.h"

/*
#include "Engine/Canvas.h"
#include "Engine/Font.h"
#include "CanvasItem.h"
*/


ARageHUD::ARageHUD()
{

}

void ARageHUD::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle MyHandle;
	GetWorldTimerManager().SetTimer(MyHandle, this, &ARageHUD::PostBeginPlay, PostDelay,false);

}

void ARageHUD::PostBeginPlay()
{
	if (GetOwningPawn() && Cast<ARagePlayerCar>(GetOwningPawn()))
	{
		Cast<ARagePlayerCar>(GetOwningPawn())->TheHUD = this;
	}
}
ARagePlayerCar* ARageHUD::GetCar()
{
	return Cast<ARagePlayerCar>(GetOwningPawn());
}
