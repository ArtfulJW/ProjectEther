// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEPlayerCharacter.h"
#include "PEPriestCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTETHER_API APEPriestCharacter : public APEPlayerCharacter
{
	GENERATED_BODY()
public:
	UFUNCTION(Server, Reliable)
	void ReplenishScrolls();

	UFUNCTION(NetMulticast, Reliable)
	void ClientReplenishResource(APEPlayerCharacter* PlayerCharacter);
};