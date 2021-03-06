// Copyright 2015-2016 Vagen Ayrapetyan

#pragma once

#include "GameFramework/PlayerState.h"
#include "RagePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class RAGE_API ARagePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void BeginPlay()override;

	UPROPERTY(Replicated,EditAnywhere, BlueprintReadWrite, Category = "")
		int32 DeathCount=0;


	UPROPERTY(Replicated,EditAnywhere, BlueprintReadWrite, Category = "")
		int32 KillCount=0;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "")
		FLinearColor PlayerColor = FLinearColor::White;

};
