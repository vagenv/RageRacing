// Copyright 2015 Vagen Ayrapetyan

#pragma once

#include "Weapons/Weapon.h"
#include "RifleWeapon.generated.h"

/**
 * 
 */
UCLASS()
class RAGE_API ARifleWeapon : public AWeapon
{
	GENERATED_BODY()

public :
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float FireHitForce = 0;

	
protected:
	virtual void Fire()override;
	
};
