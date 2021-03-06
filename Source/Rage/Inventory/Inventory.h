// Copyright 2015-2016 Vagen Ayrapetyan
#pragma once


#include "Components/ActorComponent.h"
#include "RageData.h"
#include "Inventory.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RAGE_API UInventory:public UActorComponent
{
	GENERATED_BODY()
public:

	// Base events
	UInventory();


	virtual void BeginPlay() override;


	// Player pointer
	UPROPERTY(Replicated)
	class ARagePlayerCar* ThePlayer;

	// Game Mode Pointer
	UPROPERTY()
	class ARageGameMode* TheGM;



	// Item Data List
	UPROPERTY(ReplicatedUsing = OnRep_ItemListUpdated, EditAnywhere, BlueprintReadOnly, Category = Inventory)
		TArray<FItemData> Items;

	// Item List Updated, Called on Clients
	UFUNCTION()
		void OnRep_ItemListUpdated();

	// Total Weight of Inventory
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly)
		float TotalWeight;

	// Check/Update Item Data List
	UFUNCTION(BlueprintCallable, Category = "Item")
		void UpdateInfo();

	// Add new Item Data, Called from BP
	UFUNCTION(BlueprintCallable, Category = "Item")
		FItemData AddNewItem(TSubclassOf<AItem> NewItem);

	// Add new Item Data Called From C++ to check
	FItemData* AddItem(TSubclassOf<AItem> NewItem);



	// Item was picked up
	void ItemPickedUp(class AItemPickup* ThePickup);

	// Use Item with Index
	UFUNCTION(BlueprintCallable, Category = "Item")
		void ActionIndex(int32 ItemID);

	// Use item With pointer
	UFUNCTION(BlueprintCallable, Category = "Item")
		void Action(AItem* ItemRef);

	// Throw Out item With Index
	UFUNCTION(BlueprintCallable, Category = "Item")
		void ThrowOutIndex(int32 ItemID);

	// Throw Out item with pointer
	UFUNCTION(BlueprintCallable, Category = "Item")
		void ThrowOut(AItem* ItemRef);

	// Remove Item with Idex From Item List 
	UFUNCTION(BlueprintCallable, Category = "Item")
		void RemoveItemIndex(int32 ItemID);

	// Rmove item with item pointer from Item List
	UFUNCTION(BlueprintCallable, Category = "Item")
		void RemoveItem(AItem* DeleteItem);

	// Load Inventory
	UFUNCTION(BlueprintCallable, Category = "Save")
		void LoadInventory();

	// Save inventory
	UFUNCTION(BlueprintCallable, Category = "Save")
		void SaveInventory();

};
