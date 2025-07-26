// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEEquipmentCache.h"
#include "PEPlayerController.h"
#include "GameFramework/GameStateBase.h"
#include "PEGameState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTETHER_API APEGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	/*
	 * TODO: REMOVE DEPRACATED AND ALL ASSOCIATED
	 */
	UPROPERTY()
	TMap<APEPlayerController*, TSubclassOf<APEPlayerCharacter>> PlayerControllerCharacterArray; 
	
	UPROPERTY()
	TArray<APEEquipmentCache*> TeamOneEquipmentCache;

	UFUNCTION(Server, Reliable)
	void AssignTeamToPlayerController(APEPlayerController* Requester);

	UFUNCTION(Server, Reliable)
	void AssignTeamToEquipmentCache(APEEquipmentCache* EquipmentCache);

	UFUNCTION(Server, Reliable)
	void AddTeamOneEquipmentCache(APEEquipmentCache* EquipmentCache);
};
