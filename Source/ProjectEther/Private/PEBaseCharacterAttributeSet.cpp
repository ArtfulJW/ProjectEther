// Fill out your copyright notice in the Description page of Project Settings.

#include "PEBaseCharacterAttributeSet.h"

#include "PEPlayerCharacter.h"
#include "Net/UnrealNetwork.h"

void UPEBaseCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPEBaseCharacterAttributeSet, MaxHealth);
	DOREPLIFETIME(UPEBaseCharacterAttributeSet, Health);
	DOREPLIFETIME(UPEBaseCharacterAttributeSet, Speed);
	DOREPLIFETIME(UPEBaseCharacterAttributeSet, AttackSpeed);
	DOREPLIFETIME(UPEBaseCharacterAttributeSet, AbilityCostMultiplier);
	DOREPLIFETIME(UPEBaseCharacterAttributeSet, CooldownMultiplier);
	DOREPLIFETIME(UPEBaseCharacterAttributeSet, DamageDirectionFront);
	DOREPLIFETIME(UPEBaseCharacterAttributeSet, DamageDirectionSide);
	DOREPLIFETIME(UPEBaseCharacterAttributeSet, DamageDirectionBack);
}

void UPEBaseCharacterAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPEBaseCharacterAttributeSet, MaxHealth, OldMaxHealth);
}

void UPEBaseCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPEBaseCharacterAttributeSet, Health, OldHealth);

	APEPlayerCharacter* PlayerCharacter = Cast<APEPlayerCharacter>(GetOuter());
	if (!IsValid(PlayerCharacter))
	{
		return;
	}
	
	UPEHealthBarWidget* HealthBarWidget = Cast<UPEHealthBarWidget>(PlayerCharacter->HealthBarWidgetComponent->GetWidget());
	if (!IsValid(HealthBarWidget))
	{
		return;
	}
	
	HealthBarWidget->ServerUpdateHealthBar(PlayerCharacter->AttributeSet);
}

void UPEBaseCharacterAttributeSet::OnRep_Speed(const FGameplayAttributeData& OldSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPEBaseCharacterAttributeSet, Speed, OldSpeed);
}

void UPEBaseCharacterAttributeSet::OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPEBaseCharacterAttributeSet, AttackSpeed, OldAttackSpeed);
}

void UPEBaseCharacterAttributeSet::OnRep_AbilityCostMultiplier(const FGameplayAttributeData& OldAbilityCostMultiplier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPEBaseCharacterAttributeSet, AbilityCostMultiplier, OldAbilityCostMultiplier);
}

void UPEBaseCharacterAttributeSet::OnRep_CooldownMultiplier(const FGameplayAttributeData& OldCooldownMultiplier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPEBaseCharacterAttributeSet, CooldownMultiplier, OldCooldownMultiplier);
}

void UPEBaseCharacterAttributeSet::OnRep_DamageDirectionFront(const FGameplayAttributeData& OldDamageDirectionFront)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPEBaseCharacterAttributeSet, DamageDirectionFront, OldDamageDirectionFront);
}

void UPEBaseCharacterAttributeSet::OnRep_DamageDirectionSide(const FGameplayAttributeData& OldDamageDirectionSide)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPEBaseCharacterAttributeSet, DamageDirectionSide, OldDamageDirectionSide);
}

void UPEBaseCharacterAttributeSet::OnRep_DamageDirectionBack(const FGameplayAttributeData& OldDamageDirectionBack)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPEBaseCharacterAttributeSet, DamageDirectionBack, OldDamageDirectionBack);
}

void UPEBaseCharacterAttributeSet::SetHealth_Implementation(float InHealth)
{
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetOwningActor());
	if (InHealth >= GetMaxHealth())
	{
		Health = GetMaxHealth();
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