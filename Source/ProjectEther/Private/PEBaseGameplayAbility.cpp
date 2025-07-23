// Fill out your copyright notice in the Description page of Project Settings.


#include "PETestGameplayAbility.h"

#include "CollisionDebugDrawingPublic.h"
#include "KismetTraceUtils.h"
#include "PEPlayerCharacter.h"
#include "PEPlayerController.h"

void UPETestGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                             const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UE_LOG(LogTemp, Warning, TEXT("%s: Server Executed Ability (1)"), *ActorInfo->OwnerActor->GetInstigatorController()->GetName())
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetOwningActorFromActorInfo());
	UCameraComponent* PlayerCamera = PC->CameraComponent;
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, PlayerCamera->GetComponentLocation(), PlayerCamera->GetForwardVector() * 10000, ECC_Pawn);
	DrawDebugLine(GetWorld(), PlayerCamera->GetComponentLocation(), PlayerCamera->GetForwardVector() * 10000, FColor::Black);


	APEPlayerCharacter* HitActor = Cast<APEPlayerCharacter>(Hit.GetActor());
	if (!HitActor)
	{
		return;
	}
	APEPlayerController* HitPlayerController = Cast<APEPlayerController>(HitActor->GetController());

	DrawDebugSphere(GetWorld(), Hit.Location, 15, 15,FColor::Black);
		
	if (HitActor && PC->GetController() != HitPlayerController)
	{
		UPEBaseCharacterAttributeSet* AttributeSet = Cast<UPEBaseCharacterAttributeSet>(HitActor->AttributeSet);
		EDamageDirection DamageDirection = PC->DetermineDamageDirection(Hit);
		float DamageMultipler = DetermineDamageMultiplier(DamageDirection);
		AttributeSet->SetHealth(AttributeSet->GetHealth() - 1 * DamageMultipler);
	
		UE_LOG(LogTemp, Warning, TEXT("%s; Health now %f, damaged from: %s"), *HitActor->GetName(), AttributeSet->GetHealth(), *EDamageDirection_ToString(DamageDirection));
	}
}

float UPETestGameplayAbility::DetermineDamageMultiplier(const EDamageDirection DamageDirection) const
{
	switch (DamageDirection)
	{
		case EDamageDirection::Front:
			return 1.0f;
		case EDamageDirection::Side:
			return 1.5f;
		case EDamageDirection::Back:
			return 2.0f;
		default:
			return 0.0f;
	}
}