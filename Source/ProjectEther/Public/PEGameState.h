// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEEquipmentCache.h"
#include "PEEther.h"
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

	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	/*
	 * TODO: REMOVE DEPRACATED AND ALL ASSOCIATED
	 */
	UPROPERTY()
	TMap<APEPlayerController*, TSubclassOf<APEPlayerCharacter>> PlayerControllerCharacterArray; 
	
	UPROPERTY(Replicated)
	TArray<APEEquipmentCache*> TeamOneEquipmentCache;

	UPROPERTY(Replicated)
	TArray<APEEquipmentCache*> TeamTwoEquipmentCache;

	UPROPERTY(Replicated)
	APEEther* Ether;

	UFUNCTION(Server, Reliable)
	void AssignTeamToPlayerController(APEPlayerController* Requester);

	UFUNCTION(Server, Reliable)
	void AssignTeamToEquipmentCache(APEEquipmentCache* EquipmentCache);

	UFUNCTION(Server, Reliable)
	void ServerAddEquipmentCache(APEEquipmentCache* EquipmentCache, ETeam EquipmentCacheTeam);

	UFUNCTION(Server, Reliable)
	void ServerAssignPlayerToTeam(APEPlayerController* Requester);

	UFUNCTION(Server, Reliable)
	void ServerAssignEther(APEEther* InEther);
};