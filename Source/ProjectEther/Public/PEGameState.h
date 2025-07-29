// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEEquipmentCache.h"
#include "PEEquipmentCacheSpawner.h"
#include "PEEther.h"
#include "PEEtherSpawner.h"
#include "PEEtherSpawnRegion.h"
#include "PEPlayerController.h"
#include "PEPlayerStart.h"
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
	APEGameState();
	
	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game State Specifications")
	float fEquipmentCacheSpawnDelay;
	
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
	APEEquipmentCacheSpawner* TeamOneEquipmentSpawner;

	UPROPERTY(Replicated, VisibleAnywhere)
	APEEquipmentCacheSpawner* TeamTwoEquipmentSpawner;
	
	UPROPERTY(Replicated, VisibleAnywhere)
	APEEther* Ether;
	
	UPROPERTY(Replicated, VisibleAnywhere)
	TArray<APEEtherSpawner*> EtherSpawners;

	UPROPERTY(Replicated, VisibleAnywhere)
	APEEtherSpawnRegion* EtherSpawnRegion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game State Specs")
	TSubclassOf<APEEther> EtherClass;

	UPROPERTY(Replicated, VisibleAnywhere, Category="Game State Specs")
	TArray<APEPlayerStart*> TeamOnePlayerStart;

	UPROPERTY(Replicated, VisibleAnywhere, Category="Game State Specs")
	TArray<APEPlayerStart*> TeamTwoPlayerStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game State Specs")
	TSubclassOf<APEEquipmentCache> EquipmentCacheClass;
	
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

	UFUNCTION(Server, Reliable)
	void SubscribePlayerStart(APEPlayerStart* PlayerStart, ETeam InTeam);

	UFUNCTION(Server, Reliable)
	void ServerSpawnPlayerCharacter(APEPlayerController* Requester);

	UFUNCTION(Server, Reliable)
	void ServerSpawnEther();

	UFUNCTION(Server, Reliable)
	void ServerClearEther();

	UFUNCTION(Server, Reliable)
	void ServerRemoveEquipmentCache(APEEquipmentCache* EquipmentCache, ETeam EquipmentCacheTeam);

	UFUNCTION(Server, Reliable)
	void SubscribeEquipmentCacheSpawner(APEEquipmentCacheSpawner* EquipmentCacheSpawner, ETeam EquipmentCacheTeam);

	UFUNCTION()
	void SpawnEquipmentCache(ETeam EquipmentCacheTeam);
};