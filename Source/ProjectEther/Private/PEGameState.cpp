// Fill out your copyright notice in the Description page of Project Settings.

#include "PEGameState.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

APEGameState::APEGameState():
fEquipmentCacheSpawnDelay(5.0f)
{
}

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

	ServerSpawnEther();
}

void APEGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APEGameState, TeamOneEquipmentCache);
	DOREPLIFETIME(APEGameState, TeamTwoEquipmentCache);
	DOREPLIFETIME(APEGameState, Ether);
	DOREPLIFETIME(APEGameState, EtherSpawners);
	DOREPLIFETIME(APEGameState, EtherSpawnRegion);
	DOREPLIFETIME(APEGameState, TeamOnePlayerStart);
	DOREPLIFETIME(APEGameState, TeamTwoPlayerStart);
}

void APEGameState::SpawnEquipmentCache(ETeam EquipmentCacheTeam)
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}
	
	APEEquipmentCache* SpawnedActor = nullptr;
	switch (EquipmentCacheTeam)
	{
	case TeamOne:
		SpawnedActor = World->SpawnActor<APEEquipmentCache>(EquipmentCacheClass, TeamOneEquipmentSpawner->GetTransform().GetLocation(), FRotator(0,0,0));
		break;
	case TeamTwo:
		SpawnedActor = World->SpawnActor<APEEquipmentCache>(EquipmentCacheClass, TeamTwoEquipmentSpawner->GetTransform().GetLocation(), FRotator(0,0,0));
		break;
	}

	if (IsValid(SpawnedActor))
	{
		SpawnedActor->Team = EquipmentCacheTeam;
	}
}

void APEGameState::SubscribeEquipmentCacheSpawner_Implementation(APEEquipmentCacheSpawner* EquipmentCacheSpawner, ETeam EquipmentCacheTeam)
{
	switch (EquipmentCacheTeam)
	{
	case TeamOne:
		TeamOneEquipmentSpawner = EquipmentCacheSpawner;
		break;
	case TeamTwo:
		TeamTwoEquipmentSpawner = EquipmentCacheSpawner;
		break;
	}
}

void APEGameState::ServerRemoveEquipmentCache_Implementation(APEEquipmentCache* EquipmentCache, ETeam EquipmentCacheTeam)
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("SpawnEquipmentCache"), EquipmentCacheTeam);
	
	switch (EquipmentCacheTeam)
	{
		case TeamOne:
			TeamOneEquipmentCache.Remove(EquipmentCache);
			break;
		case TeamTwo:
			TeamTwoEquipmentCache.Remove(EquipmentCache);
			break;
	}

	if (TeamOneEquipmentCache.IsEmpty() || TeamTwoEquipmentCache.IsEmpty())
	{
		World->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, fEquipmentCacheSpawnDelay, false);
	}
}

void APEGameState::ServerClearEther_Implementation()
{
	Ether = nullptr;
}

void APEGameState::ServerSpawnEther_Implementation()
{
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

void APEGameState::ServerSpawnPlayerCharacter_Implementation(APEPlayerController* Requester, EClassType ClassType)
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}
	
	TArray<APEPlayerStart*> CorrectTeam;
	switch (Requester->Team)
	{
		case TeamOne:
			CorrectTeam = TeamOnePlayerStart;
			break;
		case TeamTwo:
			CorrectTeam = TeamTwoPlayerStart;
			break;
		default:
			return;
	}
	
	int SpawningIndex = FMath::RandRange(0, CorrectTeam.Max() - 1);
	APEPlayerStart* SelectedPlayerStart = CorrectTeam[SpawningIndex];
	if (!IsValid(SelectedPlayerStart))
	{
		return;
	}
	
	APEPlayerCharacter* SpawnedPlayerCharacter = nullptr;

	switch (ClassType)
	{
		case Base:
			SpawnedPlayerCharacter = World->SpawnActor<APEPlayerCharacter>(Requester->PEPlayerCharacterClass, SelectedPlayerStart->GetTransform().GetLocation(), SelectedPlayerStart->GetTransform().Rotator());
			break;
		case Berserker:
			SpawnedPlayerCharacter = World->SpawnActor<APEPlayerCharacter>(Requester->PEBerserkerPlayerCharacterClass, SelectedPlayerStart->GetTransform().GetLocation(), SelectedPlayerStart->GetTransform().Rotator());
			break;
		case Mage:
			SpawnedPlayerCharacter = World->SpawnActor<APEPlayerCharacter>(Requester->PEMagePlayerCharacterClass, SelectedPlayerStart->GetTransform().GetLocation(), SelectedPlayerStart->GetTransform().Rotator());
			break;
		case Priest:
			SpawnedPlayerCharacter = World->SpawnActor<APEPlayerCharacter>(Requester->PEPriestPlayerCharacterClass, SelectedPlayerStart->GetTransform().GetLocation(), SelectedPlayerStart->GetTransform().Rotator());
			break;
	}
	
	if (!IsValid(SpawnedPlayerCharacter))
	{
		return;
	}
	
	Requester->Possess(SpawnedPlayerCharacter);
}

void APEGameState::SubscribePlayerStart_Implementation(APEPlayerStart* PlayerStart, const ETeam InTeam)
{
	if (!IsValid(PlayerStart))
	{
		return;
	}
	
	switch (InTeam)
	{
		case TeamOne:
			TeamOnePlayerStart.Add(PlayerStart);
			return;
		case TeamTwo:
			TeamTwoPlayerStart.Add(PlayerStart);
			return;
	}
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