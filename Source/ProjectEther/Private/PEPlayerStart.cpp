// Fill out your copyright notice in the Description page of Project Settings.

#include "PEPlayerStart.h"
#include "PEGameState.h"
#include "Kismet/GameplayStatics.h"

void APEPlayerStart::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}
	
	APEGameState* GameState = Cast<APEGameState>(UGameplayStatics::GetGameState(World));
	if (!IsValid(GameState))
	{
		return;
	}

	GameState->SubscribePlayerStart(this, Team);
}
