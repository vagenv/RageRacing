// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class RAGE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:

	AProjectile(const class FObjectInitializer& PCIP);


	void BeginPlay()override;

	bool bInstantExplode = false;
	void EnableProjectile();



	AActor* TheCar;
	AActor* TheWeapon;




	// weapon mesh: 1st person view //
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* Mesh;

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	class UProjectileMovementComponent* ProjectileMovement;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float EnableDelay = 0.2;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		bool bAutoExplodeAfterTime=true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float LifeTime = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float AffectArea = 1500;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float RadialImpulse = 600000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float CarImpulseMuliplier = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float RadialDamage = 60;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float InnerDamageRadius = 300;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float OuterDamageRadius = 500;




	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		TSubclassOf<class UDamageType> ExplosionDamageType;



	/** called when projectile hits something */
	UFUNCTION()
		void OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void InitVelocity(const FVector& ShootDirection);


	virtual void Explode();
	UFUNCTION(BlueprintImplementableEvent, Category = " Main Events ")
		void BP_Explode();
	
};
