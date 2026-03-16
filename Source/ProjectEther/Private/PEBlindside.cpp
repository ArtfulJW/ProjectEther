// Fill out your copyright notice in the Description page of Project Settings.


#include "PEBlindside.h"

#include "PEPlayerController.h"

void UPEBlindside::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
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
		UE_LOG(LogTemp, Warning, TEXT("%s: Blindside not ready"), *ActorInfo->OwnerActor->GetInstigatorController()->GetName())
		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("%s: Server Executed Ability Blindside"), *ActorInfo->OwnerActor->GetInstigatorController()->GetName())

	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetOwningActorFromActorInfo());
	if (!IsValid(PC))
	{
		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		AbilityCooldownComponent->ServerSetIsReadyToExecute(false);
		return;
	}
		
	UCameraComponent* PlayerCamera = PC->CameraComponent;
	if (!IsValid(PlayerCamera))
	{
		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		AbilityCooldownComponent->ServerSetIsReadyToExecute(false);
		return;
	}
	
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentLocation() + PlayerCamera->GetForwardVector() * fRange, ECC_Pawn);
	DrawDebugLine(GetWorld(), PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentLocation() + PlayerCamera->GetForwardVector() * fRange, FColor::Red);

	APEPlayerCharacter* HitActor = Cast<APEPlayerCharacter>(Hit.GetActor());
	if (!HitActor)
	{
		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		AbilityCooldownComponent->ServerSetIsReadyToExecute(false);
		return;
	}

	if (!IsValid(DazedGameplayEffect))
	{
		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		AbilityCooldownComponent->ServerSetIsReadyToExecute(false);
		return;
	}
	ConstructGameplayEffectSpec();
	
	if (DazedEffectSpec)
	{
		ActiveDazedEffectHandle = HitActor->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*DazedEffectSpec);
	}

	DamageHealth(HitActor, Hit, fDamageAmount);
	// PC->OnEnemyHitDelegate.ExecuteIfBound(HitActor);
	// HitActor->OnTakeDamageDelegate.ExecuteIfBound();
		
	UPEHealthBarWidget* HealthBarWidget = Cast<UPEHealthBarWidget>(HitActor->HealthBarWidgetComponent->GetWidget());
	if (!IsValid(HealthBarWidget))
	{
		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		AbilityCooldownComponent->ServerSetIsReadyToExecute(false);
		return;
	}
		
	HealthBarWidget->ServerUpdateHealthBar(HitActor->AttributeSet);
	
	AbilityCooldownComponent->ServerSetIsReadyToExecute(false);

	EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
	
	if (!IsValid(PC->PlayerHUD))
	{
		return;
	}
	PC->PlayerHUD->ActivateHitMarker();
}

void UPEBlindside::ConstructGameplayEffectSpec()
{
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetOwningActorFromActorInfo());
	if (!IsValid(PC))
	{
		UE_LOG(LogTemp, Warning, TEXT("PEBlindside::ConstructGameplayEffectSpec, Player Character is invalid"));
	}
	
	EffectContext = PC->AbilitySystemComponent->MakeEffectContext();
	DazedEffectHandle = PC->AbilitySystemComponent->MakeOutgoingSpec(DazedGameplayEffect, 1.0f, EffectContext);

	if (!DazedEffectHandle.IsValid())
	{
		return;
	}

	DazedEffectSpec = DazedEffectHandle.Data.Get();	
}