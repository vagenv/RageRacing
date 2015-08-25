// Fill out your copyright notice in the Description page of Project Settings.

#include "Rage.h"
#include "Weapon.h"
#include "Engine.h"
#include "RageBaseCar.h"

AWeapon::AWeapon(const class FObjectInitializer& PCIP) :AItem(PCIP)
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("TheRoot"));

	TheStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	TheStaticMeshComponent->AttachParent = RootComponent;



}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	bShooting = false;
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

