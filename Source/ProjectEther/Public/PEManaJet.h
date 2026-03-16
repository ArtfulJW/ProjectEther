// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEBaseGameplayAbility.h"
#include "PEManaJet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTETHER_API UPEManaJet : public UPEBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability Specs")
	TSubclassOf<UGameplayEffect> ManaJetGameplayEffect;

	FGameplayEffectContextHandle EffectContext;
	
	FGameplayEffectSpecHandle ManaJetEffectHandle;

	FActiveGameplayEffectHandle ActiveManaJetEffectHandle;

	void ConstructGameplayEffectSpec(APEPlayerCharacter* TargetPlayer);

	void ApplyBuff(APEPlayerCharacter* TargetPlayer);
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};