// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEPlayerCharacter.h"
#include "UPEAbilityCooldownComponent.h"
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
	UPEBaseGameplayAbility(const FObjectInitializer& ObjectInitializer);

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category="Ability Spec")
	TSubclassOf<UPEAbilityCooldownComponent> AbilityCooldownComponentClass;

	UPROPERTY()
	UPEAbilityCooldownComponent* AbilityCooldownComponent;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual APEPlayerCharacter* LineTraceAbility(const APEPlayerCharacter* PC, const FGameplayAbilityActorInfo* ActorInfo, float fRange, FColor Color);

	virtual void DamageHealth(APEPlayerCharacter* HitActor, FHitResult Hit, float fAmount);

	virtual void HealHealth(APEPlayerCharacter* HitActor, FHitResult Hit, float fAmount);

	virtual void ConstructGameplayEffectSpec() PURE_VIRTUAL(UPEBaseGameplayAbility::ConstructGameplayEffectSpec);
	
	float DetermineDamageMultiplier(const EDamageDirection DamageDirection, APEPlayerCharacter* PC) const;
};