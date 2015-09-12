// Fill out your copyright notice in the Description page of Project Settings.

#include "Rage.h"
#include "Engine.h"


#include "Weapons/Weapon.h"
#include "Vehicle/RagePlayerCar.h"

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
	if (WeaponData.ClipSize>0 && WeaponData.CurrentAmmo>0)
	{
		return WeaponData.CurrentAmmo / WeaponData.ClipSize;
	}

	return 0;
}


bool AWeapon::FireStart_Validate()
{
	return true;
}
void AWeapon::FireStart_Implementation()
{
	if (CanShoot())
	{
		bFirePressed = true;
		if (!GetWorldTimerManager().IsTimerActive(FireHandle))
			GetWorldTimerManager().SetTimer(FireHandle, this, &AWeapon::PreFire, WeaponData.FireSpeed, true, 0);
	}
	
	//printg("Fire Start");
}

bool AWeapon::FireEnd_Validate()
{
	return true;
}
void AWeapon::FireEnd_Implementation()
{
	bFirePressed = false;
	//printg("Fire End");
	//GetWorldTimerManager().ClearTimer(FireHandle);
}


/*
bool AWeapon::Server_Fire_Validate()
{
	return true;
}
void AWeapon::Server_Fire_Implementation()
{
	BP_Server_Fire();
}

*/

bool AWeapon::Global_Fire_Validate()
{
	return true;
}
void AWeapon::Global_Fire_Implementation()
{
	BP_Fire();
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
	if (WeaponData.FireCost>0 && WeaponData.FireCost>WeaponData.CurrentAmmo)
	{
		GetWorldTimerManager().ClearTimer(FireHandle);

		if (TheCar && Cast<ARagePlayerCar>(TheCar))
			Cast<ARagePlayerCar>(TheCar)->UnequipWeapon((uint8)WeaponType);


		//printr("Destroy Weapon. No Ammo");
		

		return;
	}

	// Call Function repetition
	GetWorldTimerManager().SetTimer(FireHandle, this, &AWeapon::PreFire, WeaponData.FireSpeed, false);

	// Actual Event
	Fire();
	//Global_Fire();
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
	UseAmmo();
	
}
bool AWeapon::CanShoot()
{
	return true;
}

bool AWeapon::HasAmmo()
{
	if (WeaponData.FireCost>0 && WeaponData.CurrentAmmo>WeaponData.FireCost )
	{
		return true;
	}


	return false;

}
void AWeapon::UseAmmo()
{
	if (WeaponData.FireCost>0)WeaponData.CurrentAmmo -= WeaponData.FireCost;
}
void AWeapon::AddAmmoValue(float AmmoNum)
{
	if (WeaponData.CurrentAmmo + AmmoNum> WeaponData.ClipSize)
	{
		WeaponData.CurrentAmmo = WeaponData.ClipSize;
	}
	else WeaponData.CurrentAmmo += AmmoNum;
}

void AWeapon::AddAmmoPickup(AWeapon* WeaponPickup)
{
	if (WeaponData.CurrentAmmo + WeaponPickup->WeaponData.CurrentAmmo > WeaponData.ClipSize)
	{
		WeaponData.CurrentAmmo = WeaponData.ClipSize;
	}
	else WeaponData.CurrentAmmo += WeaponPickup->WeaponData.CurrentAmmo;
}