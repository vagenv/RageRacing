// Copyright 2015-2016 Vagen Ayrapetyan

#pragma once

#include "Weapons/Weapon.h"
#include "ShotgunWeapon.generated.h"

/**
 * 
 */
UCLASS()
class RAGE_API AShotgunWeapon : public AWeapon
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float FireHitForce = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float SpreadValue;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		int32 FiresPerShot;

protected:
	virtual void Fire()override;
	
	
};
