// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEEtherWarStructs.h"
#include "PEInteractableBase.h"
#include "PEPlayerController.h"
#include "PEEquipmentCache.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTETHER_API APEEquipmentCache : public APEInteractableBase
{
	GENERATED_BODY()

public:

	APEEquipmentCache();

	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void Deploy() override;

	void SpawnPlayer(APEPlayerController* Requester);
	
	UPROPERTY(Replicated)
	bool bIsDeployed;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Equipment Cache Specs")
	int NumRevives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Cache Specs")
	TSubclassOf<APEPlayerCharacter> PlayerCharacter;

	UPROPERTY(Replicated)
	TEnumAsByte<ETeam> Team;
};