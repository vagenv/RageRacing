// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "ActionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RAGE_API UActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UActionComponent();


	/*

	UFUNCTION(BlueprintImplementableEvent, Category = " ")

	UFUNCTION(BlueprintCallable, Category = " ")

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = " ")

	*/

	class ARageBaseCar* TheCar;


	virtual void BeginPlay() override;
	
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;


	// BindExternal Calling
	void PreAction();
	void PreAltAction();
	void PreBoost();



protected:

	// Callabale action Events
	UFUNCTION(BlueprintImplementableEvent, Category = " Main Events ")
		void BP_PreAction();
	UFUNCTION(BlueprintImplementableEvent, Category = " Main Events ")
		void BP_Action();

	UFUNCTION(BlueprintImplementableEvent, Category = " Main Events ")
		void BP_PreAltAction();
	UFUNCTION(BlueprintImplementableEvent, Category = " Main Events ")
		void BP_AltAction();

	UFUNCTION(BlueprintImplementableEvent, Category = " Main Events ")
		void BP_PreBoost();
	UFUNCTION(BlueprintImplementableEvent, Category = " Main Events ")
		void BP_Boost();



private:


	// Internal Action Event
	void Action();
	void AltAction();
	void Boost();

	bool CanBoost();
	bool CanAction();
	bool CanAltAction();



		
	
};
