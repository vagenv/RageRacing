// Copyright 2015-2016 Vagen Ayrapetyan

#pragma once
#include "GameFramework/GameMode.h"
#include "RageGameMode.generated.h"

UCLASS(minimalapi)
class ARageGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ARageGameMode();



	virtual void BeginPlay()override;
	virtual void PostBeginPlay();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	// The Save Game file
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SaveGame")
	class USystemSaveGame* SaveFile;


};



