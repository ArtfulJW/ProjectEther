// Fill out your copyright notice in the Description page of Project Settings.


#include "PEDeployRations.h"

#include "PERations.h"

void UPEDeployRations::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!IsValid(AbilityCooldownComponentClass))
	{
		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		return;
	}

	if (!AbilityCooldownComponent->CheckIsAbilityReady())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: DeployRations not ready"), *ActorInfo->OwnerActor->GetInstigatorController()->GetName())
		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("%s: Server Executed Ability DeployRations"), *ActorInfo->OwnerActor->GetInstigatorController()->GetName())

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
	GetWorld()->LineTraceSingleByChannel(Hit, PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentLocation() + PlayerCamera->GetForwardVector() * fRange, ECC_Visibility);
	DrawDebugLine(GetWorld(), PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentLocation() + PlayerCamera->GetForwardVector() * fRange, FColor::Red);
	
	if (!IsValid(RefreshedGameplayEffect))
	{
		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	ConstructGameplayEffectSpec();

	if (!BP_RationActor)
	{
		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	DrawDebugSphere(GetWorld(), Hit.Location, 15, 15, FColor::Green);
	// APERations* SpawnedRations = GetWorld()->SpawnActor<APERations>(BP_RationActor, Hit.Location, Hit.Normal.Rotation());
	// SpawnedRations->RefreshedEffectSpec = RefreshedEffectSpec;

	if (!Hit.HasValidHitObjectHandle())
	{
		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		AbilityCooldownComponent->ServerSetIsReadyToExecute(false);
	}
	
	if (!GetActorInfo().IsNetAuthority())
	{
		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	ServerSpawnRations(Hit);
	EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
	AbilityCooldownComponent->ServerSetIsReadyToExecute(false);
}

void UPEDeployRations::ConstructGameplayEffectSpec()
{
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetOwningActorFromActorInfo());
	if (!IsValid(PC))
	{
		UE_LOG(LogTemp, Warning, TEXT("PEDeployRations::ConstructGameplayEffectSpec, Player Character is invalid"));
	}
	
	EffectContext = PC->AbilitySystemComponent->MakeEffectContext();
	RefreshedEffectHandle = PC->AbilitySystemComponent->MakeOutgoingSpec(RefreshedGameplayEffect, 1.0f, EffectContext);
}

void UPEDeployRations::ServerSpawnRations_Implementation(FHitResult Hit)
{
	APERations* SpawnedRations = GetWorld()->SpawnActor<APERations>(BP_RationActor, Hit.Location, Hit.Normal.Rotation());
	SpawnedRations->RefreshedEffectSpecHandle = RefreshedEffectHandle;
}
