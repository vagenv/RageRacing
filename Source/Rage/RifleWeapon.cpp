// Fill out your copyright notice in the Description page of Project Settings.

#include "Rage.h"
#include "RifleWeapon.h"
#include "RageBaseCar.h"
#include "Engine.h"



void ARifleWeapon::Fire()
{
	Super::Fire();

	

	const FVector StartTrace = TheStaticMeshComponent->GetSocketLocation(TEXT("MuzzleFlashSocket"));
	//	CamLoc; // trace start is the camera location
	const FVector Direction = GetActorForwardVector();
	const FVector EndTrace = StartTrace + Direction * FireDistance;

	// Perform trace to retrieve hit info
	FCollisionQueryParams TraceParams(FName(TEXT("Weapon Fire Trace")), true, this);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.AddIgnoredActor(GetOwner());


	if (DrawDebugData)DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Red, false, 3, 0, 3);

	//print("Fire Event");
	FHitResult Hit(ForceInit);
	if (GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_WorldStatic, TraceParams))
	{
		//printr("Hit Something");
		//printr(Hit.GetActor()->GetName());

		
		if (Hit.GetComponent()->IsSimulatingPhysics())Hit.GetComponent()->AddForce(-Hit.ImpactNormal*FireHitForce);

		ARageBaseCar* EnemyCar = Cast<ARageBaseCar>(Hit.GetActor()); // typecast to the item class to the hit actor
		if (EnemyCar)
		{

			//printr("Apply Damage");
			//EnemyPlayer->TakeDamage(Player, currentWeapon->MainDamage, 0);
			EnemyCar->TakeDamage(FireDamage, FDamageEvent(), NULL, this);

		}
	}

}





