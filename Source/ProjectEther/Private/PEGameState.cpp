// Fill out your copyright notice in the Description page of Project Settings.

#include "PEGameState.h"

#include "PEMathLibrary.h"
#include "PEPlayerState.h"
#include "DSP/AudioDebuggingUtilities.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

APEGameState::APEGameState():
fEquipmentCacheSpawnDelay(5.0f),
TeamOneEtherDeposit(nullptr),
TeamTwoEtherDeposit(nullptr),
CurrentSourceSpawner(nullptr),
TeamOneNumberEtherDeposited(0),
TeamTwoNumberEtherDeposited(0)
{
}

void APEGameState::AssignTeamToPlayerController_Implementation(APEPlayerController* Requester)
{
	Requester->Team = ETeam::TeamOne;
}

void APEGameState::AssignTeamToEquipmentCache_Implementation(APEEquipmentCache* EquipmentCache)
{
	EquipmentCache->Team = ETeam::TeamOne;
}

void APEGameState::ServerAddEquipmentCache_Implementation(APEEquipmentCache* EquipmentCache, const ETeam EquipmentCacheTeam)
{
	switch (EquipmentCacheTeam)
	{
		case ETeam::TeamOne:
			TeamOneEquipmentCache.Add(EquipmentCache);
			break;
		case ETeam::TeamTwo:
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
	DOREPLIFETIME(APEGameState, TeamOneEtherDeposit);
	DOREPLIFETIME(APEGameState, TeamTwoEtherDeposit);
	DOREPLIFETIME(APEGameState, CurrentSourceSpawner);
	DOREPLIFETIME(APEGameState, TeamOneNumberEtherDeposited);
	DOREPLIFETIME(APEGameState, TeamTwoNumberEtherDeposited);
}

void APEGameState::SpawnEquipmentCache_Implementation(ETeam EquipmentCacheTeam)
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}
	
	APEEquipmentCache* SpawnedActor = nullptr;
	FTransform SpawnTransform;
	switch (EquipmentCacheTeam)
	{
	case ETeam::TeamOne:
		// SpawnedActor = World->SpawnActor<APEEquipmentCache>(EquipmentCacheClass, TeamOneEquipmentSpawner->GetTransform().GetLocation(), FRotator(0,0,0));
		SpawnTransform = TeamOneEquipmentSpawner->GetTransform();
		SpawnedActor = World->SpawnActorDeferred<APEEquipmentCache>(EquipmentCacheClass, TeamOneEquipmentSpawner->GetTransform(), this);
		break;
	case ETeam::TeamTwo:
		// SpawnedActor = World->SpawnActor<APEEquipmentCache>(EquipmentCacheClass, TeamTwoEquipmentSpawner->GetTransform().GetLocation(), FRotator(0,0,0));
		SpawnTransform = TeamTwoEquipmentSpawner->GetTransform();
		SpawnedActor = World->SpawnActorDeferred<APEEquipmentCache>(EquipmentCacheClass, TeamTwoEquipmentSpawner->GetTransform(), this);
		break;
	}

	if (IsValid(SpawnedActor))
	{
		SpawnedActor->Team = EquipmentCacheTeam;
	}

	 UGameplayStatics::FinishSpawningActor(SpawnedActor,SpawnTransform);
}

void APEGameState::ServerSubscribeEtherDeposit_Implementation(APEEtherDeposit* InEtherDeposit, ETeam InEtherDepositTeam)
{
	switch (InEtherDepositTeam)
	{
		case ETeam::TeamOne:
			TeamOneEtherDeposit = InEtherDeposit;
			break;
		case ETeam::TeamTwo:
			TeamTwoEtherDeposit = InEtherDeposit;
			break;
	}

	// Force Server to immediately update values for the Client instead of waiting for the next Replication Tick
	ForceNetUpdate();
}

ETeam APEGameState::DetermineEtherClosestTeam(float& fEtherDistanceToGoal, float& TotalDistance)
{
	if (!IsValid(Ether) || !IsValid(TeamOneEtherDeposit) || !IsValid(TeamTwoEtherDeposit) || !IsValid(CurrentSourceSpawner))
	{
		return ETeam::NoTeam;
	}
	
	float fEtherDistanceToTeamOne = PEMathLibrary::EuclideanDistance(Ether->GetActorLocation(), TeamOneEtherDeposit->GetActorLocation());
	float fEtherDistanceToTeamTwo = PEMathLibrary::EuclideanDistance(Ether->GetActorLocation(), TeamTwoEtherDeposit->GetActorLocation());

	if (fEtherDistanceToTeamOne > fEtherDistanceToTeamTwo)
	{
		TotalDistance = PEMathLibrary::EuclideanDistance(CurrentSourceSpawner->GetActorLocation(), TeamTwoEtherDeposit->GetActorLocation());
		fEtherDistanceToGoal = fEtherDistanceToTeamTwo;
		return ETeam::TeamTwo;
	}
	else if (fEtherDistanceToTeamOne == fEtherDistanceToTeamTwo)
	{
		return ETeam::NoTeam;
	}
	else
	{
		TotalDistance = PEMathLibrary::EuclideanDistance(CurrentSourceSpawner->GetActorLocation(), TeamOneEtherDeposit->GetActorLocation());
		fEtherDistanceToGoal = fEtherDistanceToTeamOne;
		return ETeam::TeamOne;
	}
}

void APEGameState::UpdateNumberOfEtherDeposited_Implementation(APEEtherDeposit* InEtherDeposit)
{
	switch (InEtherDeposit->Team)
	{
		case ETeam::TeamOne:
			TeamOneNumberEtherDeposited = InEtherDeposit->NumDepositedEther;
			return;
		case ETeam::TeamTwo:
			TeamTwoNumberEtherDeposited = InEtherDeposit->NumDepositedEther;
			return;
		case ETeam::NoTeam:
			return;
	}
}

TArray<APEEquipmentCache*> APEGameState::GetTeamEquipmentCacheArray(ETeam InTeam)
{
	switch (InTeam)
	{
		case ETeam::TeamOne:
			return TeamOneEquipmentCache;
		case ETeam::TeamTwo:
			return TeamTwoEquipmentCache;
	}
	return {};
}

void APEGameState::SubscribeEquipmentCacheSpawner_Implementation(APEEquipmentCacheSpawner* EquipmentCacheSpawner, ETeam EquipmentCacheTeam)
{
	switch (EquipmentCacheTeam)
	{
	case ETeam::TeamOne:
		TeamOneEquipmentSpawner = EquipmentCacheSpawner;
		break;
	case ETeam::TeamTwo:
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
		case ETeam::TeamOne:
			TeamOneEquipmentCache.Remove(EquipmentCache);
			break;
		case ETeam::TeamTwo:
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

	FVector SpawnLocation;
	if (EtherSpawners.Num() > 0)
	{
		int SpawningIndex = FMath::RandRange(0, EtherSpawners.Num() - 1);

		// Check for out of index errors
		if (EtherSpawners.Num() == 0 || SpawningIndex < 0 || SpawningIndex > EtherSpawners.Num() - 1)
		{
			// No EtherSpawner location provided, default to SpawnRegion's location
			UE_LOG(LogTemp, Warning, TEXT("SpawningIndex out of bounds"))
			SpawnLocation = EtherSpawnRegion->GetActorLocation();
		}
		else
		{
			CurrentSourceSpawner = EtherSpawners[SpawningIndex];
			SpawnLocation = CurrentSourceSpawner->GetActorLocation();
		}
	}

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
		case ETeam::TeamOne:
			CorrectTeam = TeamOnePlayerStart;
			break;
		case ETeam::TeamTwo:
			CorrectTeam = TeamTwoPlayerStart;
			break;
		default:
			return;
	}
	
	int SpawningIndex = FMath::RandRange(0, CorrectTeam.Num() - 1);
	APEPlayerStart* SelectedPlayerStart = CorrectTeam[SpawningIndex];
	if (!IsValid(SelectedPlayerStart))
	{
		return;
	}
	
	APEPlayerCharacter* SpawnedPlayerCharacter = nullptr;

	switch (ClassType)
	{
		case EClassType::Base:
			SpawnedPlayerCharacter = World->SpawnActor<APEPlayerCharacter>(Requester->PEPlayerCharacterClass, SelectedPlayerStart->GetTransform().GetLocation(), SelectedPlayerStart->GetTransform().Rotator());
			break;
		case EClassType::Berserker:
			SpawnedPlayerCharacter = World->SpawnActor<APEPlayerCharacter>(Requester->PEBerserkerPlayerCharacterClass, SelectedPlayerStart->GetTransform().GetLocation(), SelectedPlayerStart->GetTransform().Rotator());
			break;
		case EClassType::Mage:
			SpawnedPlayerCharacter = World->SpawnActor<APEPlayerCharacter>(Requester->PEMagePlayerCharacterClass, SelectedPlayerStart->GetTransform().GetLocation(), SelectedPlayerStart->GetTransform().Rotator());
			break;
		case EClassType::Priest:
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
		case ETeam::TeamOne:
			TeamOnePlayerStart.Add(PlayerStart);
			return;
		case ETeam::TeamTwo:
			TeamTwoPlayerStart.Add(PlayerStart);
			return;
	}
}

void APEGameState::ServerSubscribeEtherSpawnRegion_Implementation(APEEtherSpawnRegion* SpawnRegion)
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}
	
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
		APEPlayerState* CachedPlayerState = Cast<APEPlayerState>(CachedPC->GetPlayerState<APEPlayerState>());
		if (CachedPC != Requester)
		{
			x++;
			continue;
		}
		
		if (x % 2 == 0)
		{
			CachedPC->Team = ETeam::TeamOne;
		}
		else
		{
			CachedPC->Team = ETeam::TeamTwo;
		}

		CachedPlayerState->ServerSetTagname(FText::FromString(ETeam_ToString(CachedPC->Team)));
		x++;
	}
}