// Copyright 2015-2016 Vagen Ayrapetyan

#include "Rage.h"
#include "Engine.h"

#include "Vehicle/RagePlayerCar.h"
#include "Weapons/ProjectileWeapon.h"
#include "Projectile.h"


void AProjectileWeapon::Fire()
{

	Super::Fire();
	// try and fire a projectile
	if (GrenadeArchetype != NULL)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			FVector tempV = TheStaticMeshComponent->GetSocketRotation(TEXT("MuzzleFlashSocket")).Vector();
			//InitialProjectileVelocity
			tempV.Normalize();
			tempV.Z += VerticalOffsetAngle;


			//	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("World got"));
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			// spawn the projectile at the muzzle
			AProjectile* theProjectile = World->SpawnActor<AProjectile>(GrenadeArchetype, 
				TheStaticMeshComponent->GetSocketLocation(TEXT("MuzzleFlashSocket")),
				tempV.Rotation() );
			theProjectile->TheWeapon = this;
			theProjectile->ThePlayer = ThePlayer;

			if (theProjectile)
			{
			
				theProjectile->InitVelocity(tempV);
			}
		}
	}
}

