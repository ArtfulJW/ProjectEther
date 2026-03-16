// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEBaseGameplayAbility.h"
#include "PEDamageLineTrace.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTETHER_API UPEDamageLineTrace : public UPEBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UPEDamageLineTrace(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Spec")
	float fDamage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Spec")
	float Range = 0.0f;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
