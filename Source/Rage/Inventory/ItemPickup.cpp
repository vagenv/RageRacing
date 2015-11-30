// Copyright 2015 Vagen Ayrapetyan

#include "Rage.h"
#include "ItemPickup.h"
#include "Vehicle/RagePlayerCar.h"
#include "Inventory/Item.h"
#include "Inventory/Inventory.h"

#include "UnrealNetwork.h"

// Sets default values
AItemPickup::AItemPickup(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{



//	RootComponent = PCIP.CreateDefaultSubobject<USceneComponent>(this, TEXT("RootComponent"));
//	RootComponent->SetIsReplicated(true);

	/*
	SkeletalMesh = PCIP.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("SkeletalMesh"));
	SkeletalMesh->AttachParent = RootComponent;
	SkeletalMesh->SetNetAddressable();
	SkeletalMesh->SetIsReplicated(true);

	SkeletalMesh->bRenderCustomDepth = true;
	SkeletalMesh->MarkRenderStateDirty();
	SkeletalMesh->SetRenderCustomDepth(false);
	*/

	Mesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	RootComponent = Mesh;
	//Mesh->AttachParent = RootComponent;
	Mesh->SetNetAddressable();
	Mesh->SetIsReplicated(true);
	Mesh->SetRenderCustomDepth(false);

	TriggerSphere = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("TriggerSphere"));
	TriggerSphere->InitSphereRadius(300);
	TriggerSphere->AttachParent = Mesh;

	bReplicates = true;
	bReplicateMovement = true;
}


void AItemPickup::BeginPlay()
{
	Super::BeginPlay();


	SetReplicateMovement(true);
	SetReplicates(true);

	// Set Pickup Skeletal Mesh Outline
	//if (SkeletalMesh)SkeletalMesh->SetRenderCustomDepth(false);

	// Set Pickup Mesh Outline
	if (Mesh)Mesh->SetRenderCustomDepth(false);



	

	//SetReplicates(true);

	/*
	if (Role < ROLE_Authority)
	{
		TriggerSphere->Deactivate();
		//TriggerSphere->DestroyComponent();
		return;
	}
	*/
	// Start Activate Delay on server
	FTimerHandle MyHandle;
	GetWorldTimerManager().SetTimer(MyHandle, this, &AItemPickup::ActivatePickupOverlap, PickupActivationDelay, false);

	if (!bOverideItemData)ActivatePickupPhysics();

}

// Activate Overlap detection
void AItemPickup::ActivatePickupOverlap()
{
	if (Role < ROLE_Authority)
		return;
	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &AItemPickup::OnBeginOverlap);
	TriggerSphere->OnComponentEndOverlap.AddDynamic(this, &AItemPickup::OnEndOverlap);
}

// Activate Pickup Physics
void AItemPickup::ActivatePickupPhysics()
{

	if (Role < ROLE_Authority)
		return;

	if (Mesh->StaticMesh)
	{
		//SkeletalMesh->DestroyComponent();
		//TriggerSphere->AttachTo(Mesh);

		Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		Mesh->SetSimulatePhysics(true);
		Mesh->WakeRigidBody();
	}
	/*
	else if (SkeletalMesh->SkeletalMesh)
	{
		Mesh->DestroyComponent();
		//TriggerSphere->AttachTo(SkeletalMesh, NAME_None, EAttachLocation::SnapToTarget);
		//TriggerSphere->AttachTo(SkeletalMesh);
		SkeletalMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		SkeletalMesh->SetSimulatePhysics(true);
		SkeletalMesh->WakeRigidBody();
	}
	*/
}

// Player Entered The Pickup Area
void AItemPickup::OnBeginOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && Cast<ARagePlayerCar>(OtherActor) != NULL && Cast<ARagePlayerCar>(OtherActor)->TheInventory != NULL)
	{
		// BP Event that player entered
		BP_PlayerEntered(Cast<ARagePlayerCar>(OtherActor));

		// Auto give player the item
		if (bAutoPickup)
		{
			PickedUp(Cast<ARagePlayerCar>(OtherActor));
		}

		// Wait player Input
		else
		{
			// Set player ref of item pickup
			Cast<ARagePlayerCar>(OtherActor)->currentPickup = this;

			// Activate highlight
			//if (SkeletalMesh)SkeletalMesh->SetRenderCustomDepth(true);
			if (Mesh)Mesh->SetRenderCustomDepth(true);
		}
	}
}

// Player Exited The Pickup Area
void AItemPickup::OnEndOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && Cast<ARagePlayerCar>(OtherActor) != NULL)
	{
		// BP Event that player Exited
		BP_PlayerExited(Cast<ARagePlayerCar>(OtherActor));

		// Clean Pickup reference in player class 
		if (Cast<ARagePlayerCar>(OtherActor)->currentPickup == this)
			Cast<ARagePlayerCar>(OtherActor)->currentPickup = NULL;

		// Disable pickup highlight
		//if (SkeletalMesh)SkeletalMesh->SetRenderCustomDepth(false);
		if (Mesh)Mesh->SetRenderCustomDepth(false);
	}
}

// The Actual event of pickup
void AItemPickup::PickedUp(ARageBaseCar * Player)
{
	// check the distance between pickup and player 
	if (Player && Item != NULL && Cast<ARagePlayerCar>(Player)->TheInventory)
	{
		//  Add item to inventory
		Cast<ARagePlayerCar>(Player)->TheInventory->ItemPickedUp(this);

		// BP event that item was picked up in player blueprint
		BP_PickedUp(Cast<ARagePlayerCar>(Player));
	}
	Destroy();
}

void AItemPickup::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AItemPickup, Mesh);
///	DOREPLIFETIME(AItemPickup, SkeletalMesh);
}