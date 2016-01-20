// Copyright 2015-2016 Vagen Ayrapetyan

#pragma once

#include "Vehicle/RagePlayerCar.h"
#include "RageDirtCar.generated.h"

/**
 * 
 */
UCLASS()
class RAGE_API ARageDirtCar : public ARagePlayerCar
{
	GENERATED_BODY()
public:
	ARageDirtCar();

	void BeginPlay()override;
};
