// Copyright 2015 Vagen Ayrapetyan

#pragma once

#include "Vehicle/RageBaseCar.h"
#include "RageEnemyCar.generated.h"

/**
 * 
 */
UCLASS()
class RAGE_API ARageEnemyCar : public ARageBaseCar
{
	GENERATED_BODY()
public:
	ARageEnemyCar();

	void BeginPlay()override;
};
