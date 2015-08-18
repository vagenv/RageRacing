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


	class ARageBaseCar* GetCar();

};
