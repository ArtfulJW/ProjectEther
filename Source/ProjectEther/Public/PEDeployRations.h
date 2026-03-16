// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEBaseGameplayAbility.h"
#include "PEInteractableBase.h"
#include "PEDeployRations.generated.h"

class APERations;
/**
 * 
 */
UCLASS()
class PROJECTETHER_API UPEDeployRations : public UPEBaseGameplayAbility
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Spec")
	float HealAmount = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Spec")
	float fRange = 1000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability Specs")
	TSubclassOf<UGameplayEffect> RefreshedGameplayEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability Specs")
	TSubclassOf<APERations> BP_RationActor;
	
	FGameplayEffectContextHandle EffectContext;

	FGameplayEffectSpecHandle RefreshedEffectHandle;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void ConstructGameplayEffectSpec() override;

	UFUNCTION(Server, Reliable)
	void ServerSpawnRations(FHitResult Hit);
};
