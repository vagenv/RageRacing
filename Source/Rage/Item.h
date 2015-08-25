// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class RAGE_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem(const class FObjectInitializer& PCIP);

	virtual void BeginPlay() override;


	// Item part
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
		UTexture2D* Icon;


	UFUNCTION(BlueprintImplementableEvent, Category = " ")
		void BP_ItemPickedUp();

	UFUNCTION(BlueprintImplementableEvent, Category = " ")
		void BP_ItemUsed();


};
