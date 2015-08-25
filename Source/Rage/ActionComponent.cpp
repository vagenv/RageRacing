// Fill out your copyright notice in the Description page of Project Settings.

#include "Rage.h"
#include "Engine.h"
#include "ActionComponent.h"
#include "RageBaseCar.h"
#include "Weapon.h"
// Sets default values for this component's properties

UActionComponent::UActionComponent()
{

	bWantsBeginPlay = true;
}

void UActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner() && Cast<ARageBaseCar>(GetOwner()))TheCar = Cast<ARageBaseCar>(GetOwner());

	if (TheCar)
	{
		if (DefaultMainWeapon)
		{

			MainWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultMainWeapon);
			MainWeapon->AttachRootComponentTo(TheCar->GetMesh(), MainWeapon->AttachSocket);
			MainWeapon->TheCar = TheCar;
			//MainWeapon->AttachActorToActor
			//MainWeapon->AttachTo(TheCar->GetMesh(), "MainWeaponSocket");

		}
		if (DefaultAltWeapon)
		{

			AltWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultAltWeapon);
			AltWeapon->AttachRootComponentTo(TheCar->GetMesh(), AltWeapon->AttachSocket);
			AltWeapon->TheCar = TheCar;
			//AltWeapon->AttachTo(TheCar->GetMesh(), "AltWeaponSocket");
		}

	}


}






void UActionComponent::ActionDown()
{
	BP_ActionDown();
	if (MainWeapon)
	{
		MainWeapon->FireStart();
	}
}
void UActionComponent::ActionUp()
{
	BP_ActionUp();
	if (MainWeapon)
	{
		MainWeapon->FireEnd();
	}
}


void UActionComponent::AltActionDown()
{
	BP_AltActionDown();
	if (AltWeapon)
	{
		AltWeapon->FireStart();
	}
}
void UActionComponent::AltActionUp()
{
	BP_AltActionUp();
	if (AltWeapon)
	{
		AltWeapon->FireEnd();
	}
}


void UActionComponent::BoostDown()
{
	BP_BoostDown();
}
void UActionComponent::BoostUp()
{
	BP_BoostUp();
}

