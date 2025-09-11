// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEBasePassiveAbilityComponent.h"
#include "Components/SphereComponent.h"
#include "PECrushingPresencePassive.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTETHER_API UPECrushingPresencePassive : public UPEBasePassiveAbilityComponent
{
	GENERATED_BODY()

public:
	UPECrushingPresencePassive();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Passive Specs")
	USphereComponent* SphereColliderComponent;	
};