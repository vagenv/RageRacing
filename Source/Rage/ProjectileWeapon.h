// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon.h"
#include "ProjectileWeapon.generated.h"

/**
 * 
 */
UCLASS()
class RAGE_API AProjectileWeapon : public AWeapon
{
	GENERATED_BODY()
public:
	virtual void Fire()override;


	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Projectile")
		TSubclassOf <class AProjectile> GrenadeArchetype;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Projectile")
		float InitialProjectileVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Projectile")
		float VerticalOffsetAngle;
	
	
};
