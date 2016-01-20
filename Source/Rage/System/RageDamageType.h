// Copyright 2015-2016 Vagen Ayrapetyan

#pragma once

#include "GameFramework/DamageType.h"
#include "RageDamageType.generated.h"

/**
 * 
 */
UCLASS()
class RAGE_API URageDamageType : public UDamageType
{
	GENERATED_BODY()
	
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageType")
		float Damage_VehicleDamageMultiplier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageType")
		float Damage_ShieldDamageMultiplier = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageType")
		float Damage_ShieldPenetrationMultiplier = 0.1f;
	
};
