// Fill out your copyright notice in the Description page of Project Settings.

#include "PEManaJet.h"

void UPEManaJet::ConstructGameplayEffectSpec(APEPlayerCharacter* TargetPlayer)
{
	// Make the effect context for the first player to leave the range of WarriorChallengeFlag
	EffectContext = TargetPlayer->AbilitySystemComponent->MakeEffectContext();

	// Create Handles for corresponding GameplayEffects
	ManaJetEffectHandle = TargetPlayer->AbilitySystemComponent->MakeOutgoingSpec(ManaJetGameplayEffect, 1.0f, EffectContext);
}

void UPEManaJet::ApplyBuff(APEPlayerCharacter* TargetPlayer)
{
	if (!ManaJetEffectHandle.IsValid())
	{
		return;
	}

	ActiveManaJetEffectHandle = TargetPlayer->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*ManaJetEffectHandle.Data.Get());
}

void UPEManaJet::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Check for valid Cooldown Component
	if (!IsValid(AbilityCooldownComponentClass))
	{
		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		return;
	}

	// Check if this ability is ready to be executed
	if (!AbilityCooldownComponent->CheckIsAbilityReady())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: ManaJet not ready"), *ActorInfo->OwnerActor->GetInstigatorController()->GetName())
		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		return;
	}

	// Ability is ready to be executed.
	UE_LOG(LogTemp, Warning, TEXT("%s: Server Executed Ability ManaJet"), *ActorInfo->OwnerActor->GetInstigatorController()->GetName())
	
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetOwningActorFromActorInfo());
	
	ConstructGameplayEffectSpec(PC);
	ApplyBuff(PC);

	EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
	AbilityCooldownComponent->ServerSetIsReadyToExecute(false);
}