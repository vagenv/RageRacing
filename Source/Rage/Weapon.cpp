// Fill out your copyright notice in the Description page of Project Settings.

#include "Rage.h"
#include "Weapon.h"
#include "Engine.h"
#include "RagePlayerCar.h"

AWeapon::AWeapon(const class FObjectInitializer& PCIP) :AItem(PCIP)
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("TheRoot"));

	TheStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	TheStaticMeshComponent->AttachParent = RootComponent;

	AttachSockets.Add(FName("MainWeaponSocket"));
	AttachSockets.Add(FName("AltWeaponSocket"));
	AttachSockets.Add(FName("UltimateWeaponSocket"));


	

}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	bShooting = false;

	if (AttachSocket.ToString().Len()<1 && AttachSockets.Num()>2)
	{
		switch (WeaponType)
		{
			case EWeaponArchetype::Main:
			{
				AttachSocket = AttachSockets[0];
				break;
			}
			case EWeaponArchetype::Secondary:
			{
				AttachSocket = AttachSockets[1];
				break;
			}
			case EWeaponArchetype::Ultimate:
			{
				AttachSocket = AttachSockets[2];
				break;
			}

		}
	}


}

float AWeapon::GetCurrentAmmoPercent()
{
	if (ClipSize>0 && CurrentAmmo>0)
	{
		return CurrentAmmo / ClipSize;
	}

	return 0;
}

void AWeapon::FireStart()
{
	if (CanShoot())
	{
		bFirePressed = true;
		if (!GetWorldTimerManager().IsTimerActive(FireHandle))
			GetWorldTimerManager().SetTimer(FireHandle, this, &AWeapon::PreFire, FireSpeed, true,0);
	}
	
}

void AWeapon::FireEnd()
{
	bFirePressed = false;
	//GetWorldTimerManager().ClearTimer(FireHandle);
}

void AWeapon::PreFire()
{
	//Stopped Firing
	if (!bFirePressed)
	{
		GetWorldTimerManager().ClearTimer(FireHandle);
		return;
	}

	// Not enough ammo
	if (FireCost>0 && FireCost>CurrentAmmo)
	{
		GetWorldTimerManager().ClearTimer(FireHandle);

		if (TheCar && Cast<ARagePlayerCar>(TheCar))
			Cast<ARagePlayerCar>(TheCar)->UnequipWeapon((uint8)WeaponType);


		//printr("Destroy Weapon. No Ammo");
		

		return;
	}

	// Call Function repetition
	GetWorldTimerManager().SetTimer(FireHandle, this, &AWeapon::PreFire, FireSpeed, false);

	// Actual Event
	Fire();

	/*
	//	Add Feedback to car
	if (TheCar)
	{
		//printg("Add Force");
		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorRotation().Vector()*1000, FColor::Green, false, 3, 0, 3);

		TheCar->GetMesh()->AddForce(-1 * GetActorRotation().Vector()*WeaponFireFeedback);
		//TheCar->GetMovementComponent()->
	}
	*/

	//	Use ammo
	if (FireCost>0)CurrentAmmo -= FireCost;
}
bool AWeapon::CanShoot()
{
	return true;
}

void AWeapon::AddAmmoValue(float AmmoNum)
{
	if (CurrentAmmo+AmmoNum>ClipSize)
	{
		CurrentAmmo = ClipSize;
	}
	else CurrentAmmo += AmmoNum;
}

void AWeapon::AddAmmoPickup(AWeapon* WeaponPickup)
{
	if (CurrentAmmo + WeaponPickup->CurrentAmmo>ClipSize)
	{
		CurrentAmmo = ClipSize;
	}
	else CurrentAmmo += WeaponPickup->CurrentAmmo;
}