// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEPlayerController.h"
#include "GameFramework/SpectatorPawn.h"
#include "PESpectatorPawn.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTETHER_API APESpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()

public:

	APESpectatorPawn();
	
	virtual void BeginPlay() override;

	float fDeathTimeDelay;

	FTimerHandle DeathTimerHandle;

	UFUNCTION(Server, Unreliable)
	void ServerRequestRevive(APEPlayerController* Requester);

	void RequestRevive();
};