// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEBaseGameplayAbility.h"
#include "PEHealingLineTrace.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTETHER_API UPEHealingLineTrace : public UPEBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Spec")
	float fHealAmount = 6.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Spec")
	float Range = 2000.0f;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
