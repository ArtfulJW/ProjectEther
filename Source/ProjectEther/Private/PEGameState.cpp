// Fill out your copyright notice in the Description page of Project Settings.

#include "PEGameState.h"

#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

void APEGameState::AssignTeamToPlayerController_Implementation(APEPlayerController* Requester)
{
	Requester->Team = TeamOne;
}

void APEGameState::AssignTeamToEquipmentCache_Implementation(APEEquipmentCache* EquipmentCache)
{
	EquipmentCache->Team = TeamOne;
}

void APEGameState::ServerAddEquipmentCache_Implementation(APEEquipmentCache* EquipmentCache, const ETeam EquipmentCacheTeam)
{
	switch (EquipmentCacheTeam)
	{
		case TeamOne:
			TeamOneEquipmentCache.Add(EquipmentCache);
			break;
		case TeamTwo:
			TeamTwoEquipmentCache.Add(EquipmentCache);
			break;
	}
}

void APEGameState::BeginPlay()
{
	Super::BeginPlay();
}

void APEGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APEGameState, TeamOneEquipmentCache);
	DOREPLIFETIME(APEGameState, TeamTwoEquipmentCache);
	DOREPLIFETIME(APEGameState, Ether);
}

void APEGameState::ServerAssignEther_Implementation(APEEther* InEther)
{
	Ether = InEther;
}

void APEGameState::ServerAssignPlayerToTeam_Implementation(APEPlayerController* Requester)
{
	int x = 0;
	for (APlayerState* PlayerState : this->PlayerArray)
	{
		APEPlayerController* CachedPC = Cast<APEPlayerController>(PlayerState->GetPlayerController());
		if (CachedPC != Requester)
		{
			x++;
			continue;
		}
		
		if (x % 2 == 0)
		{
			CachedPC->Team = TeamOne;
		}
		else
		{
			CachedPC->Team = TeamTwo;
		}
		
		x++;
	}
}
