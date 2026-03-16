// Fill out your copyright notice in the Description page of Project Settings.


#include "PERations.h"

APERations::APERations():
	DeployRationsGameplayAbility(nullptr),
	ActiveRefreshedEffectHandle(NULL),
	RefreshedEffectSpecHandle(nullptr)
{
	bReplicates = true;
}

void APERations::Interact(APEPlayerCharacter& InteractingPlayerCharacter)
{
	// Make Effect and heal the player
	if (RefreshedEffectSpecHandle.IsValid())
	{
		ActiveRefreshedEffectHandle = InteractingPlayerCharacter.AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*RefreshedEffectSpecHandle.Data.Get());
	}
	InteractingPlayerCharacter.AttributeSet->SetHealth(InteractingPlayerCharacter.AttributeSet->GetHealth() + DeployRationsGameplayAbility.GetDefaultObject()->HealAmount);
	
	// Destroy this Ration
	MulticastDestroyRation();
}

void APERations::MulticastDestroyRation_Implementation()
{
	Destroy();
}
