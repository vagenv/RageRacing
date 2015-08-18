// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Rage.h"
#include "RageHUD.h"
#include "RageBaseCar.h"

/*
#include "Engine/Canvas.h"
#include "Engine/Font.h"
#include "CanvasItem.h"
*/


ARageHUD::ARageHUD()
{

}


ARageBaseCar* ARageHUD::GetCar()
{
	return Cast<ARageBaseCar>(GetOwningPawn());
}
