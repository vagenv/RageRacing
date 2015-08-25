// Fill out your copyright notice in the Description page of Project Settings.

#include "Rage.h"
#include "Engine.h"
#include "ProjectileWeapon.h"
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

			//	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("World got"));
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			// spawn the projectile at the muzzle
			AProjectile* theProjectile = World->SpawnActor<AProjectile>(GrenadeArchetype, 
				TheStaticMeshComponent->GetSocketLocation(TEXT("MuzzleFlashSocket")),
				TheStaticMeshComponent->GetSocketRotation(TEXT("MuzzleFlashSocket")));

			//, , GetActorRightVector().Rotation(), SpawnParams
			//theProjectile->SetActorLocation(Mesh1P->GetSocketLocation(TEXT("MuzzleFlashSocket")));


			

			//DrawDebugLine(GetWorld(), theProjectile->GetActorLocation(), theProjectile->GetActorLocation() + theProjectile->GetActorForwardVector() * 500, FColor::Red, false, 3, 0, 1 );
			/*
			*/

			if (theProjectile)
			{
				theProjectile->InitVelocity(TheStaticMeshComponent->GetSocketRotation(TEXT("MuzzleFlashSocket")).Vector()*InitialProjectileVelocity);
				//theProjectile->InitVelocity(TheStaticMeshComponent->GetSocketRotation(TEXT("MuzzleFlashSocket")).Vector()*InitialProjectileVelocity);
			}
		}
	}
}

