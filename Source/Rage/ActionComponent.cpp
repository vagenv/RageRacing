// Fill out your copyright notice in the Description page of Project Settings.

#include "Rage.h"
#include "ActionComponent.h"
#include "RageBaseCar.h"

// Sets default values for this component's properties
UActionComponent::UActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UActionComponent::PreAction()
{
	BP_PreAction();
	if (CanAction())Action();
}

void UActionComponent::Action()
{
	BP_Action();
	
}

void UActionComponent::PreAltAction()
{
	BP_PreAltAction();
	if (CanAltAction())AltAction();
}

void UActionComponent::AltAction()
{
	BP_AltAction();
}

void UActionComponent::PreBoost()
{
	BP_PreBoost();
	if (CanBoost())Boost();
}
void UActionComponent::Boost()
{
	BP_Boost();
}

bool UActionComponent::CanBoost()
{
	return true;
}

bool UActionComponent::CanAction()
{
	return true;
}

bool UActionComponent::CanAltAction()
{
	return true;
}






// Called when the game starts
void UActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UActionComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

