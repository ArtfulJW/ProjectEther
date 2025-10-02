// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Components/ActorComponent.h"
#include "PEBasePassiveAbilityActor.generated.h"

UCLASS()
class PROJECTETHER_API APEBasePassiveAbilityActor : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	APEBasePassiveAbilityActor();
	
	virtual void SetupPassiveAbility();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;
};