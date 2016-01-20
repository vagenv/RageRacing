// Copyright 2015-2016 Vagen Ayrapetyan

#include "Rage.h"
#include "Engine.h"

#include "Weapons/RifleWeapon.h"
#include "Vehicle/RagePlayerCar.h"



void ARifleWeapon::Fire()
{
	Super::Fire();

//	printr("Rifle Fire");
	if (ThePlayer && ThePlayer->Controller)
	{
		const FVector StartTrace = TheStaticMeshComponent->GetSocketLocation(TEXT("MuzzleFlashSocket"));
		//	CamLoc; // trace start is the camera location
		const FVector Direction = ThePlayer->GetActorForwardVector();
		const FVector EndTrace = StartTrace + Direction * WeaponData.FireDistance;

		// Perform trace to retrieve hit info
		FCollisionQueryParams TraceParams(FName(TEXT("Weapon Fire Trace")), true, this);
		TraceParams.bTraceAsyncScene = true;
		TraceParams.bReturnPhysicalMaterial = true;
		TraceParams.AddIgnoredActor(GetOwner());


		DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Red, false, 5, 0, 4);

		//print("Fire Event");
		FHitResult Hit(ForceInit);
		if (GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_WorldStatic, TraceParams))
		{
			//printr("Hit Something");
		//	printg(Hit.GetActor()->GetName());


			if (Hit.GetComponent()->IsSimulatingPhysics())Hit.GetComponent()->AddForce(-Hit.ImpactNormal*FireHitForce);

			ARageBaseCar* EnemyCar = Cast<ARageBaseCar>(Hit.GetActor()); // typecast to the item class to the hit actor
			if (EnemyCar)
			{

				//printr("Apply Damage");
				//EnemyPlayer->TakeDamage(Player, currentWeapon->MainDamage, 0);
				//EnemyCar->TakeDamage(FireDamage, FDamageEvent(), NULL, this);
				//EnemyCar->ApplyDamageMomentum(WeaponData.FireDamage, FDamageEvent(), NULL,NULL);
				// Apply Damage
				UGameplayStatics::ApplyDamage(EnemyCar, WeaponData.FireDamage, ThePlayer->Controller, Cast<AActor>(this), DamageType);

			}
			//	else printr("Bad Cast");
		}
	}
	

}





