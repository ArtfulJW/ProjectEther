// Fill out your copyright notice in the Description page of Project Settings.


#include "PEHealingLineTrace.h"

#include "PEPlayerController.h"

void UPEHealingLineTrace::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Warning, TEXT("%s: Server Executed Ability HealingLineTrace"), *ActorInfo->OwnerActor->GetInstigatorController()->GetName())
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetOwningActorFromActorInfo());
	UCameraComponent* PlayerCamera = PC->CameraComponent;
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentLocation() + PlayerCamera->GetForwardVector() * 2000, ECC_Pawn);
	DrawDebugLine(GetWorld(), PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentLocation() + PlayerCamera->GetForwardVector() * 2000, FColor::Green);

	APEPlayerCharacter* HitActor = Cast<APEPlayerCharacter>(Hit.GetActor());
	if (!HitActor)
	{
		return;
	}
	APEPlayerController* HitPlayerController = Cast<APEPlayerController>(HitActor->GetController());
	DrawDebugSphere(GetWorld(), Hit.Location, 15, 15,FColor::Green);

	if (HitActor && PC->GetController() != HitPlayerController)
	{
		HealHealth(HitActor, Hit, 3.0f);
		HitActor->MulticastUpdateHealthBar();
	}
}
