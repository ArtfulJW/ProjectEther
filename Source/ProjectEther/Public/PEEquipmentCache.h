// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEInteractableBase.h"
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

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void Deploy() override;

	UPROPERTY(Replicated)
	bool bIsDeployed;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Equipment Cache Specs")
	int NumRevives;
};