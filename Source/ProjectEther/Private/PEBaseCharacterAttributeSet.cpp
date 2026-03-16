// Fill out your copyright notice in the Description page of Project Settings.

#include "PEBaseCharacterAttributeSet.h"

#include "PEPlayerCharacter.h"
#include "Net/UnrealNetwork.h"

void UPEBaseCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPEBaseCharacterAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPEBaseCharacterAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPEBaseCharacterAttributeSet, Speed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPEBaseCharacterAttributeSet, AttackSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPEBaseCharacterAttributeSet, AbilityCostMultiplier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPEBaseCharacterAttributeSet, CooldownMultiplier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPEBaseCharacterAttributeSet, DamageDirectionFront, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPEBaseCharacterAttributeSet, DamageDirectionSide, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPEBaseCharacterAttributeSet, DamageDirectionBack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPEBaseCharacterAttributeSet, JumpMagnitude, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPEBaseCharacterAttributeSet, Resource, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPEBaseCharacterAttributeSet, ResourceMaxValue, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPEBaseCharacterAttributeSet, ResourceReplenishRate, COND_None, REPNOTIFY_Always);
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

	// TODO: Update personal Health bar here
	if (IsValid(PlayerCharacter->PlayerHUD))
	{
		PlayerCharacter->PlayerHUD->UpdateHUDHealthBar(PlayerCharacter->AttributeSet);
	}

	if (Health.GetCurrentValue() <= 0.0f)
	{
		PlayerCharacter->ClientCleanupClientWeapon();
	}
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

void UPEBaseCharacterAttributeSet::OnRep_JumpMagnitude(const FGameplayAttributeData& OldJumpMagnitude)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPEBaseCharacterAttributeSet, JumpMagnitude, OldJumpMagnitude);
}

void UPEBaseCharacterAttributeSet::OnRep_Resource(const FGameplayAttributeData& OldResource)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPEBaseCharacterAttributeSet, Resource, OldResource);

	APEPlayerCharacter* PlayerCharacter = Cast<APEPlayerCharacter>(GetOuter());
	if (!IsValid(PlayerCharacter))
	{
		return;
	}
}

void UPEBaseCharacterAttributeSet::OnRep_ResourceReplenishRate(const FGameplayAttributeData& OldResourceReplenishRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPEBaseCharacterAttributeSet, ResourceReplenishRate, OldResourceReplenishRate);
}

void UPEBaseCharacterAttributeSet::OnRep_ResourceMaxValue(const FGameplayAttributeData& OldResourceMaxValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPEBaseCharacterAttributeSet, ResourceMaxValue, OldResourceMaxValue);
}

void UPEBaseCharacterAttributeSet::SetResource_Implementation(float InAmount)
{
	if (UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
	{
		ASC->SetNumericAttributeBase(GetResourceAttribute(), FMath::Clamp(InAmount, 0.0f, GetResourceMaxValue()));
	}
	
	// APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetOwningActor());
	// if (InAmount >= PC->AttributeSet->GetResourceMaxValue())
	// {
	// 	Resource = PC->AttributeSet->GetResourceMaxValue();
	// }
	// else
	// {
	// 	Resource = InAmount;
	// }
}

void UPEBaseCharacterAttributeSet::AddResource_Implementation(float InAmount)
{
	if (UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
	{
		ASC->SetNumericAttributeBase(GetResourceAttribute(), FMath::Clamp(GetResource() + InAmount, 0.0f, GetResourceMaxValue()));
	}
	
	// APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetOwningActor());
	// if (PC->AttributeSet->GetResource() + InAmount >= PC->AttributeSet->GetResourceMaxValue())
	// {
	// 	Resource = PC->AttributeSet->GetResourceMaxValue();
	// }
	// else
	// {
	// 	Resource = PC->AttributeSet->GetResource() + InAmount;
	// }
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
		Health = 0.0f;
		OnRep_Health(Health);
		PC->BeforeDestroy();
		PC->Destroy();
	}
}
