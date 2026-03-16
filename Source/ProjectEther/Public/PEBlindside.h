// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEBaseGameplayAbility.h"
#include "PEBlindside.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTETHER_API UPEBlindside : public UPEBaseGameplayAbility
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Spec")
	float fDamageAmount = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Spec")
	float fRange = 1000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Specs")
	TSubclassOf<UGameplayEffect> DazedGameplayEffect;

	FGameplayEffectContextHandle EffectContext;

	FGameplayEffectSpecHandle DazedEffectHandle;
	
	FActiveGameplayEffectHandle ActiveDazedEffectHandle;

	FGameplayEffectSpec* DazedEffectSpec;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void ConstructGameplayEffectSpec() override;
};