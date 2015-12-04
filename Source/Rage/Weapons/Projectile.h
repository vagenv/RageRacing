// Copyright 2015 Vagen Ayrapetyan

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



	APawn* ThePlayer;
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

	// Projectile Affect Area
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float AffectArea = 1500;

	// Projectile Explosion Damage Type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		TSubclassOf<class UDamageType> ExplosionDamageType;

	// Radial Damage Curve
	UPROPERTY(EditAnywhere, Category = "Projectile")
		FRuntimeFloatCurve RadialDamageCurve;

	// Radial Impulse Curve
	UPROPERTY(EditAnywhere, Category = "Projectile")
		FRuntimeFloatCurve RadialImpulseCurve;



	/** called when projectile hits something */
	UFUNCTION()
		void OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void InitVelocity(const FVector& ShootDirection);


	virtual void Explode();
	UFUNCTION(BlueprintImplementableEvent, Category = " Main Events ")
		void BP_Explode();


	// BP Hit
	UFUNCTION(BlueprintImplementableEvent, Category = "Projectile")
		void BP_Hit(UPrimitiveComponent* OtherComp, const FHitResult& Hit);

	// Explosion hit Enemy
	UFUNCTION(BlueprintImplementableEvent, Category = "Projectile")
		void BP_Explode_HitEnemy(class ARageBaseCar* TheCharacter, float HitDamage);
	
};
