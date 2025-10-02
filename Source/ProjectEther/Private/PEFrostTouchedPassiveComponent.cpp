// Fill out your copyright notice in the Description page of Project Settings.


#include "PEFrostTouchedPassiveComponent.h"

APEFrostTouchedPassiveComponent::APEFrostTouchedPassiveComponent():
bBindToPlayerOnHitDelegate(true)
{
}

void APEFrostTouchedPassiveComponent::OnHit(APEPlayerCharacter* HitPlayer)
{
	ServerApplyOnHitGameplayEffect(HitPlayer);
}

void APEFrostTouchedPassiveComponent::BeginPlay()
{
	Super::BeginPlay();
}

void APEFrostTouchedPassiveComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APEFrostTouchedPassiveComponent::SetupPassiveAbility()
{
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetOwner());
	if (!IsValid(PC))
	{
		return;
	}

	if (bBindToPlayerOnHitDelegate)
	{
		PC->OnEnemyHitDelegate.BindUObject(this, &APEFrostTouchedPassiveComponent::OnHit);
	}
}

FGameplayEffectSpec APEFrostTouchedPassiveComponent::ConstructGameplayEffect(FGameplayEffectContextHandle InContextHandle,
FGameplayEffectSpecHandle InSpecHandle, TSubclassOf<UGameplayEffect> InGameplayEffect) const
{
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetOwner());
	if (!IsValid(PC))
	{
		UE_LOG(LogTemp, Warning, TEXT("CrushingPresence::OnHit, Invalid PlayerCharacter"));
	}
	
	InContextHandle = PC->AbilitySystemComponent->MakeEffectContext();
	InSpecHandle = PC->AbilitySystemComponent->MakeOutgoingSpec(InGameplayEffect, 1.0f, InContextHandle);
	if (!InSpecHandle.IsValid())
	{
		return FGameplayEffectSpec();
	}
	return *InSpecHandle.Data.Get();
}

void APEFrostTouchedPassiveComponent::ServerApplyOnHitGameplayEffect_Implementation(
	const APEPlayerCharacter* OverlappedPlayerCharacter)
{
	if (!IsValid(OverlappedPlayerCharacter))
	{
		return;
	}
	
	FrostTouchedEffectSpec = ConstructGameplayEffect(FrostTouchedContextHandle, FrostTouchedEffectSpecHandle, GE_Frost);
	ActiveFrostTouchedEffectHandle = OverlappedPlayerCharacter->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(FrostTouchedEffectSpec);

	// Updates the PlayerCharacter with their own health, if MaxHealth ever becomes lower than their current health.
	OverlappedPlayerCharacter->AttributeSet->SetHealth(OverlappedPlayerCharacter->AttributeSet->GetHealth());

	// Update Healthbar widget
	UPEHealthBarWidget* HealthBarWidget = Cast<UPEHealthBarWidget>(OverlappedPlayerCharacter->HealthBarWidgetComponent->GetWidget());
	if (!IsValid(HealthBarWidget))
	{
		return;
	}
	HealthBarWidget->ServerUpdateHealthBar(OverlappedPlayerCharacter->AttributeSet);
}
