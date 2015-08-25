// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon.h"
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
