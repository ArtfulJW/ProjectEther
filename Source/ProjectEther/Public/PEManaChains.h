// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEBaseGameplayAbility.h"
#include "PEManaChains.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTETHER_API UPEManaChains : public UPEBaseGameplayAbility
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Spec")
	float fDamageAmount = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Spec")
	float fRange = 3000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Specs")
	TSubclassOf<UGameplayEffect> ChainedGameplayEffect;

	FGameplayEffectContextHandle EffectContext;

	FGameplayEffectSpecHandle ChainedEffectHandle;
	
	FActiveGameplayEffectHandle ActiveChainedEffectHandle;

	FGameplayEffectSpec* ChainedEffectSpec;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
