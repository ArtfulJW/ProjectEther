// Fill out your copyright notice in the Description page of Project Settings.


#include "PETestGameplayAbility.h"

void UPETestGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UE_LOG(LogTemp, Warning, TEXT("%s: Server Executed Ability (1)"), *ActorInfo->OwnerActor->GetInstigatorController()->GetName())
}
