// Fill out your copyright notice in the Description page of Project Settings.


#include "PEHealingLineTrace.h"

#include "PEPlayerController.h"

void UPEHealingLineTrace::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	if (!IsValid(AbilityCooldownComponentClass))
	{
		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		return;
	}

	if (!AbilityCooldownComponent->CheckIsAbilityReady())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: HealingLineTrace not ready"), *ActorInfo->OwnerActor->GetInstigatorController()->GetName())
		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("%s: Server Executed Ability HealingLineTrace"), *ActorInfo->OwnerActor->GetInstigatorController()->GetName())
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetOwningActorFromActorInfo());
	UCameraComponent* PlayerCamera = PC->CameraComponent;
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentLocation() + PlayerCamera->GetForwardVector() * Range, ECC_Pawn);
	DrawDebugLine(GetWorld(), PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentLocation() + PlayerCamera->GetForwardVector() * Range, FColor::Green);

	APEPlayerCharacter* HitActor = Cast<APEPlayerCharacter>(Hit.GetActor());
	if (!HitActor)
	{
		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		AbilityCooldownComponent->ServerSetIsReadyToExecute(false);
		return;
	}
	APEPlayerController* HitPlayerController = Cast<APEPlayerController>(HitActor->GetController());
	// DrawDebugSphere(GetWorld(), Hit.Location, 15, 15,FColor::Green);

	if (HitActor && PC->GetController() != HitPlayerController)
	{
		HealHealth(HitActor, Hit, fHealAmount);
		
		UPEHealthBarWidget* HealthBarWidget = Cast<UPEHealthBarWidget>(HitActor->HealthBarWidgetComponent->GetWidget());
		if (!IsValid(HealthBarWidget))
		{
			EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
			AbilityCooldownComponent->ServerSetIsReadyToExecute(false);
			return;
		}
		
		HealthBarWidget->ServerUpdateHealthBar(HitActor->AttributeSet);

		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		AbilityCooldownComponent->ServerSetIsReadyToExecute(false);
		
		if (!IsValid(PC->PlayerHUD))
		{
			EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
			return;
		}
		PC->PlayerHUD->ActivateHitMarker();
	}
}