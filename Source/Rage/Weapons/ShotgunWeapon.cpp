// Fill out your copyright notice in the Description page of Project Settings.

#include "Rage.h"
#include "Engine.h"

#include "Weapons/ShotgunWeapon.h"
#include "Vehicle/RageBaseCar.h"



void AShotgunWeapon::Fire()
{
	

	Super::Fire();
	// Casting 


	const FVector StartTrace = TheStaticMeshComponent->GetSocketLocation(TEXT("MuzzleFlashSocket"));
	const FVector Direction = GetActorForwardVector();
	const FVector EndTrace = StartTrace + Direction * FireDistance;


	float localx;
	float localy;

	FVector offsetVector;
	for (int32 i = 0; i < FiresPerShot; i++)
	{
		localx = FMath::FRandRange(-1, 1);
		localy = FMath::FRandRange(-1, 1);
		if (FMath::Pow(localx, 2) + FMath::Pow(localy, 2) > 1)
		{
			localx /= 2;
			localy /= 2;
		}

		// Cast lines
		offsetVector = GetActorRightVector()*localx*SpreadValue + GetActorUpVector()*localy*SpreadValue;


		if (DrawDebugData)DrawDebugLine(GetWorld(), StartTrace, EndTrace + offsetVector, FColor::Red, false, 3, 0, 3);


		// Perform trace to retrieve hit info
		FCollisionQueryParams TraceParams(FName(TEXT("WeaponTrace")), true, this);
		TraceParams.bTraceAsyncScene = true;
		TraceParams.bReturnPhysicalMaterial = true;
		TraceParams.AddIgnoredActor(GetOwner());
		//print("Fire Event");
		FHitResult Hit(ForceInit);
		if (GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace + offsetVector, ECC_WorldStatic, TraceParams))
		{
			//printr("Hit Something");
			//printr(Hit.GetActor()->GetName());


			if (Hit.GetComponent()->IsSimulatingPhysics())Hit.GetComponent()->AddForce(-Hit.ImpactNormal*FireHitForce);

			ARageBaseCar* EnemyCar = Cast<ARageBaseCar>(Hit.GetActor()); // typecast to the item class to the hit actor
			if (EnemyCar)
			{

				//printr("Apply Damage");
				//EnemyPlayer->TakeDamage(Player, currentWeapon->MainDamage, 0);
				//EnemyCar->TakeDamage(FireDamage, FDamageEvent(), NULL, this);

			}
		}


	}




	///////////////////////









}

