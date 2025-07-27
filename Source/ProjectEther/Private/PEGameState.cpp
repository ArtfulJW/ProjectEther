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

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}
	
	int SpawningIndex = FMath::RandRange(0, EtherSpawners.Max() - 1);
	FVector SpawnLocation = EtherSpawners[SpawningIndex]->GetActorLocation();

	if (!Ether)
	{
		Ether = World->SpawnActor<APEEther>(EtherClass, SpawnLocation, FRotator(0, 0, 0));
	}
}

void APEGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APEGameState, TeamOneEquipmentCache);
	DOREPLIFETIME(APEGameState, TeamTwoEquipmentCache);
	DOREPLIFETIME(APEGameState, Ether);
	DOREPLIFETIME(APEGameState, EtherSpawners);
	DOREPLIFETIME(APEGameState, EtherSpawnRegion);
}

void APEGameState::ServerSubscribeEtherSpawnRegion_Implementation(APEEtherSpawnRegion* SpawnRegion)
{
	if (!IsValid(SpawnRegion))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid EtherSpawnRegion: %s"), *SpawnRegion->GetName());
		return;
	}
	
	EtherSpawnRegion = SpawnRegion;
}

void APEGameState::ServerSubscribeEtherSpawner_Implementation(APEEtherSpawner* Spawner)
{
	if (!IsValid(Spawner))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Spawner: %s"), *Spawner->GetName());
		return;
	}

	EtherSpawners.Add(Spawner);
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