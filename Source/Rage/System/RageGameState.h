// Copyright 2015-2016 Vagen Ayrapetyan

#pragma once

#include "GameFramework/GameState.h"
#include "RageData.h"
#include "RageGameState.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSimpleEventDelegate);

UCLASS()
class RAGE_API ARageGameState : public AGameState
{
	GENERATED_BODY()
	

public:


	// Constructor
	ARageGameState();
	virtual void BeginPlay();


	UPROPERTY(ReplicatedUsing = OnRep_MessagesList, EditAnywhere, BlueprintReadOnly, Category = " ")
	TArray<FRageOnlineMessageData> TheMessages;

	UFUNCTION()
		void OnRep_MessagesList();

	UPROPERTY(BlueprintAssignable, Category = "")
		FSimpleEventDelegate ChatUpdateDelegate;

	void AddNewChatMessage(FString TheMessage, class ARagePlayerCar* ThePlayer);


};
