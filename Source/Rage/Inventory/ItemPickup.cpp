// Fill out your copyright notice in the Description page of Project Settings.

#include "Rage.h"
#include "ItemPickup.h"
#include "Vehicle/RagePlayerCar.h"
#include "Item.h"
// Sets default values
AItemPickup::AItemPickup(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	

	Mesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	RootComponent=Mesh;

	TriggerBox = PCIP.CreateDefaultSubobject<UBoxComponent>(this, TEXT("Trigger BOX"));
	TriggerBox->InitBoxExtent(FVector(100, 100, 100));
	TriggerBox->AttachParent = Mesh;
	bReplicates = true;
}


void AItemPickup::BeginPlay()
{
	Super::BeginPlay();

	if (Role>=ROLE_Authority)
	{

		TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AItemPickup::OnBeginOverlap);
	}
	else TriggerBox->DestroyComponent();

}

void AItemPickup::OnBeginOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Item != NULL && Item->GetDefaultObject<AItem>()&& OtherActor != NULL && OtherActor != this && OtherComp != NULL && Cast<ARagePlayerCar>(OtherActor) != NULL)
	{
		Cast<ARagePlayerCar>(OtherActor)->ItemPickup(Item);
		BP_ItemPickedUp();
		Destroy();

	}
}
