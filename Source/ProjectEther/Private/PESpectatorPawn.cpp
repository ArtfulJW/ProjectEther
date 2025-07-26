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

	GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, this, &APESpectatorPawn::RequestRevive, fDeathTimeDelay, false);
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
	APEEquipmentCache* EquipmentCache = Cast<APEEquipmentCache>(GameState->TeamOneEquipmentCache[0]);
	EquipmentCache->SpawnPlayer(Requester);
}
