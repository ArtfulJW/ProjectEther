// Fill out your copyright notice in the Description page of Project Settings.


#include "UPEAbilityCooldownComponent.h"

#include "PEBaseGameplayAbility.h"
#include "PEPlayerController.h"

UPEAbilityCooldownComponent::UPEAbilityCooldownComponent():
	bIsReadyToExecute(true)
{
}

void UPEAbilityCooldownComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPEAbilityCooldownComponent, bIsReadyToExecute);
}

bool UPEAbilityCooldownComponent::CheckIsAbilityReady()
{
	return bIsReadyToExecute;
}

void UPEAbilityCooldownComponent::ServerSetIsReadyToExecute_Implementation(bool inBool)
{
	bIsReadyToExecute = inBool;
	
	if (!inBool)
	{
		RefreshAbilityCooldown();
	}
	else
	{
		TimerHandle.Invalidate();
	}
}

void UPEAbilityCooldownComponent::RefreshAbilityCooldown()
{
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("ServerSetIsReadyToExecute"), true);

	UPEBaseGameplayAbility* BaseGameplayAbility = Cast<UPEBaseGameplayAbility>(GetOuter());
	if (!IsValid(BaseGameplayAbility))
	{
		return;
	}
	
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(BaseGameplayAbility->GetOwningActorFromActorInfo());
	if (!IsValid(PC))
	{
		return;
	}

	UWorld* World = PC->GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	if (!TimerHandle.IsValid())
	{
		float ClampedAttackSpeed = FMath::Clamp(PC->AttributeSet->GetAttackSpeed(), 0.0f , 100.0f);
		float OutAbilitySpeed = FMath::Clamp((fCooldownTime * PC->AttributeSet->GetCooldownMultiplier()) - ClampedAttackSpeed, fMinCooldownTime, fMaxCooldownTime);
		World->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, OutAbilitySpeed, false);
	}
}

void UPEAbilityCooldownComponent::OnRep_ReadyChanged(bool inBool)
{
	bIsReadyToExecute = inBool;
}
