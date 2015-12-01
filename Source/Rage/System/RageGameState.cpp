// Copyright 2015 Vagen Ayrapetyan

#include "Rage.h"
#include "Engine.h"
#include "RageGameState.h"
#include "UnrealNetwork.h"



ARageGameState::ARageGameState()
{
	bReplicates = true;
}

void ARageGameState::BeginPlay()
{
	Super::BeginPlay();
}

// Called On Client to Update HUD when message list updated
void ARageGameState::OnRep_MessagesList()
{
	ChatUpdateDelegate.Broadcast();
}
// Called on server to add new message
void ARageGameState::AddNewChatMessage(FString  TheMessage, class ARagePlayerCar* ThePlayer)
{
	TheMessages.Add(FRageOnineMessageData(TheMessage, ThePlayer));


	// Update HUD on server
	ChatUpdateDelegate.Broadcast();
}



// Called On Client to Update HUD when Player list updated
void ARageGameState::OnRep_PlayersListUpdated()
{
	PlayerUpdateDelegate.Broadcast();
}


void ARageGameState::UpdatePlayerStats()
{

}


// Replication of data
void ARageGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARageGameState, TheMessages);
}