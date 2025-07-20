// Fill out your copyright notice in the Description page of Project Settings.


#include "PEBaseCharacterAttributeSet.h"

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
