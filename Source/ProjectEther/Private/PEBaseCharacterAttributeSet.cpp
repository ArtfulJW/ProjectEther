// Fill out your copyright notice in the Description page of Project Settings.

#include "PEBaseCharacterAttributeSet.h"

#include "PEPlayerCharacter.h"
#include "Net/UnrealNetwork.h"

void UPEBaseCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPEBaseCharacterAttributeSet, Health);
	DOREPLIFETIME(UPEBaseCharacterAttributeSet, Speed);
}

void UPEBaseCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPEBaseCharacterAttributeSet, Health, OldHealth);
}

void UPEBaseCharacterAttributeSet::OnRep_Speed(const FGameplayAttributeData& OldSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPEBaseCharacterAttributeSet, Health, OldSpeed);
}

void UPEBaseCharacterAttributeSet::SetHealth_Implementation(float InHealth)
{
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetOwningActor());
	if (InHealth >= 100.0f)
	{
		Health = 100.0f;
	}
	else
	{
		Health = InHealth;
	}

	if (Health.GetCurrentValue() <= 0.0f)
	{
		PC->BeforeDestroy();
		PC->Destroy();
	}
}