// Fill out your copyright notice in the Description page of Project Settings.


#include "PEManaChains.h"

#include "PE_GEChained.h"

void UPEManaChains::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!IsValid(AbilityCooldownComponentClass))
	{
		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		return;
	}

	if (!AbilityCooldownComponent->CheckIsAbilityReady())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: ManaChains not ready"), *ActorInfo->OwnerActor->GetInstigatorController()->GetName())
		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("%s: Server Executed Ability Mana Chains"), *ActorInfo->OwnerActor->GetInstigatorController()->GetName())

	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetOwningActorFromActorInfo());
	if (!IsValid(PC))
	{
		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		return;
	}
		
	UCameraComponent* PlayerCamera = PC->CameraComponent;
	if (!IsValid(PlayerCamera))
	{
		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentLocation() + PlayerCamera->GetForwardVector() * fRange, ECC_Pawn);
	DrawDebugLine(GetWorld(), PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentLocation() + PlayerCamera->GetForwardVector() * fRange, FColor::Blue);

	APEPlayerCharacter* HitActor = Cast<APEPlayerCharacter>(Hit.GetActor());
	if (!HitActor)
	{
		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		AbilityCooldownComponent->ServerSetIsReadyToExecute(false);
		return;
	}

	if (!IsValid(ChainedGameplayEffect))
	{
		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		return;
	}

	// Constructing the Chained Gameplay Effect
	EffectContext = PC->AbilitySystemComponent->MakeEffectContext();
	ChainedEffectHandle = PC->AbilitySystemComponent->MakeOutgoingSpec(ChainedGameplayEffect, 1.0f, EffectContext);
	EffectContext.AddOrigin(HitActor->GetActorLocation());
	if (!ChainedEffectHandle.IsValid())
	{
		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		return;
	}
	ChainedEffectSpec = ChainedEffectHandle.Data.Get();

	// Applying the Gameplay Effect to the Hit Player
	if (ChainedEffectSpec)
	{
		ActiveChainedEffectHandle = HitActor->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*ChainedEffectSpec);
	}

	// Damaging the Hit Player
	DamageHealth(HitActor, Hit, fDamageAmount);

	// Updating health bars
	UPEHealthBarWidget* HealthBarWidget = Cast<UPEHealthBarWidget>(HitActor->HealthBarWidgetComponent->GetWidget());
	if (!IsValid(HealthBarWidget))
	{
		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		return;
	}
	HealthBarWidget->ServerUpdateHealthBar(HitActor->AttributeSet);
	AbilityCooldownComponent->ServerSetIsReadyToExecute(false);
	EndAbility(Handle,ActorInfo, ActivationInfo, true, false);

	// Triggering Hit Marker
	if (!IsValid(PC->PlayerHUD))
	{
		return;
	}
	PC->PlayerHUD->ActivateHitMarker();
}
