// Copyright 2015 Vagen Ayrapetyan

#include "Rage.h"
#include "RagePlayerState.h"




void ARagePlayerState::BeginPlay()
{
	Super::BeginPlay();
	KillCount = 0;
	DeathCount = 0;
}


// Replication of data
void ARagePlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARagePlayerState, DeathCount);
	DOREPLIFETIME(ARagePlayerState, KillCount);
	DOREPLIFETIME(ARagePlayerState, PlayerColor);
}