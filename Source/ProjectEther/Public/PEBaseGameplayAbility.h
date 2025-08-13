// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEPlayerCharacter.h"
#include "Abilities/GameplayAbility.h"
#include "PEBaseGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTETHER_API UPEBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual APEPlayerCharacter* LineTraceAbility(const APEPlayerCharacter* PC, const FGameplayAbilityActorInfo* ActorInfo, float fRange, FColor Color);

	virtual void DamageHealth(APEPlayerCharacter* PC, FHitResult Hit, float fAmount);

	virtual void HealHealth(APEPlayerCharacter* PC, FHitResult Hit, float fAmount);
	
	float DetermineDamageMultiplier(const EDamageDirection DamageDirection) const;
};