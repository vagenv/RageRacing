// Copyright 2015 Vagen Ayrapetyan

#pragma once

#include "GameFramework/Actor.h"
#include "RageData.h"
#include "Item.generated.h"

UCLASS()
class RAGE_API AItem : public AActor
{
	GENERATED_BODY()


public:
		//			Basic Events
		AItem(const class FObjectInitializer& PCIP);

	virtual void BeginPlay() override;


	// Pickup Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Pickup")
		UStaticMesh* PickupMesh;

	// Pickup Skeletal mesg
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Pickup")
		USkeletalMesh* PickupSkelMesh;

	// Player Pointer
	UPROPERTY(Replicated)
	class ARagePlayerCar* ThePlayer;


	// Item Name
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
		FString ItemName = FString("Name of Item");

	// Item Icon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
		UTexture2D* ItemIcon;

	// Item Count
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
		int32 ItemCount = 1;

	// Item Weight
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
		float Weight = 1;

	// Item Pickup Override Archtype
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
		TSubclassOf<class AItemPickup> ItemPickupArchetype;


	//			 Item Events

	// BP Event- Item Was Used
	UFUNCTION(BlueprintImplementableEvent, Category = "Item")
		void BP_ItemUsed();

	// BP Event- Item Was Dropped
	UFUNCTION(BlueprintImplementableEvent, Category = "Item")
		void BP_ItemDroped(class AItemPickup* thePickup);

	// BP Event- Item Was Updated
	UFUNCTION(BlueprintImplementableEvent, Category = "Item")
		void BP_ItemUpdated();

	// BP Event- Item Was picked up
	UFUNCTION(BlueprintImplementableEvent, Category = "Item")
		void BP_ItemPickedUp(class AItemPickup* ThePickup);


	// Get Player Ref
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "The HUD")
	class ARagePlayerCar* GetPlayer()const;


	// Get Player Ref
	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void RemoveThisItemFromInventory();
	// Get Player Ref
	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void DropThisItemFromInventory();

	// Get World Ref
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "The HUD")
	class UWorld* GetPlayerWorld()const;


	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item")
	class AItem* ItemClassToItemRef(TSubclassOf<class AItem> TheItemClass)const;


	// Item Was Used

	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void InventoryUse(class ARagePlayerCar* Player)
	{
		// Set Player Ref if player ref in empty or wrong
		if (ThePlayer == NULL || ThePlayer != Player)
		{
			ThePlayer = Player;
		}

		// BP event called
		BP_ItemUsed();
	};

};
