// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEEquipmentCache.h"
#include "PEEther.h"
#include "PEEtherSpawner.h"
#include "PEEtherSpawnRegion.h"
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
	
	UPROPERTY(Replicated, VisibleAnywhere)
	TArray<APEEquipmentCache*> TeamOneEquipmentCache;

	UPROPERTY(Replicated, VisibleAnywhere)
	TArray<APEEquipmentCache*> TeamTwoEquipmentCache;

	UPROPERTY(Replicated, VisibleAnywhere)
	APEEther* Ether;

	UPROPERTY(Replicated, VisibleAnywhere)
	TArray<APEEtherSpawner*> EtherSpawners;

	UPROPERTY(Replicated, VisibleAnywhere)
	APEEtherSpawnRegion* EtherSpawnRegion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game State Specs")
	TSubclassOf<APEEther> EtherClass;

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

	UFUNCTION(Server, Reliable)
	void ServerSubscribeEtherSpawner(APEEtherSpawner* Spawner);

	UFUNCTION(Server, Reliable)
	void ServerSubscribeEtherSpawnRegion(APEEtherSpawnRegion* SpawnRegion);
};