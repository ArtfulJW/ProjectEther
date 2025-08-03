// Fill out your copyright notice in the Description page of Project Settings.

#include "PESpectatorPawn.h"

#include "PEEquipmentCache.h"
#include "PEGameState.h"
#include "Kismet/GameplayStatics.h"

APESpectatorPawn::APESpectatorPawn():
fDeathTimeDelay(7.0f)
{
}

void APESpectatorPawn::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, this, &APESpectatorPawn::RequestRevive, fDeathTimeDelay, true);
}

void APESpectatorPawn::RequestRevive()
{
	if (GetNetMode() >= NM_Client)
	{
		APEPlayerController* PlayerController = Cast<APEPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		ServerRequestRevive(PlayerController);
	}
}

void APESpectatorPawn::ServerRequestRevive_Implementation(APEPlayerController* Requester)
{
	if (!IsValid(Requester))
	{
		return;
	}

	APEGameState* GameState = Cast<APEGameState>(UGameplayStatics::GetGameState(GetWorld()));
	APEPlayerController* PlayerController = Cast<APEPlayerController>(this->GetController());
	if (GameState->TeamOneEquipmentCache.IsEmpty() || GameState->TeamTwoEquipmentCache.IsEmpty())
	{
		GameState->ServerSpawnPlayerCharacter(PlayerController, PlayerController->CharacterClass);
		return;
	}
	
	APEEquipmentCache* EquipmentCache = nullptr;
	switch (Requester->Team)
	{
		case TeamOne:
			for (APEEquipmentCache* EquipmentCacheArrayItem : GameState->TeamOneEquipmentCache)
			{
				if (EquipmentCacheArrayItem->bIsDeployed)
				{
					EquipmentCache = EquipmentCacheArrayItem;
					break;
				}
			}
			// EquipmentCache = Cast<APEEquipmentCache>(GameState->TeamOneEquipmentCache[0]);
			break;
		case TeamTwo:
			for (APEEquipmentCache* EquipmentCacheArrayItem : GameState->TeamTwoEquipmentCache)
			{
				if (EquipmentCacheArrayItem->bIsDeployed)
				{
					EquipmentCache = EquipmentCacheArrayItem;
					break;
				}
			}
			// EquipmentCache = Cast<APEEquipmentCache>(GameState->TeamTwoEquipmentCache[0]);
			break;
	}
	
	if (IsValid(EquipmentCache) && EquipmentCache->bIsDeployed)
	{
		EquipmentCache->SpawnPlayer(Requester);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Undeployed Team EquipmentCache, spawning at main base"))
		GameState->ServerSpawnPlayerCharacter(PlayerController, PlayerController->CharacterClass);
	}
}
