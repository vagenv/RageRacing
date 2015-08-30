// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Rage.h"
#include "RageHUD.h"
#include "RagePlayerCar.h"

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


}
ARagePlayerCar* ARageHUD::GetCar()
{
	return Cast<ARagePlayerCar>(GetOwningPawn());
}
