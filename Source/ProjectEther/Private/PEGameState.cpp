// Fill out your copyright notice in the Description page of Project Settings.

#include "PEGameState.h"

void APEGameState::AssignTeamToPlayerController_Implementation(APEPlayerController* Requester)
{
	Requester->Team = TeamOne;
}

void APEGameState::AssignTeamToEquipmentCache_Implementation(APEEquipmentCache* EquipmentCache)
{
	EquipmentCache->Team = TeamOne;
}

void APEGameState::AddTeamOneEquipmentCache_Implementation(APEEquipmentCache* EquipmentCache)
{
	TeamOneEquipmentCache.Add(EquipmentCache);
}