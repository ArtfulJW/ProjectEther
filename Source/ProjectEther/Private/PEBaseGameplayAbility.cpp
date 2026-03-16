// Fill out your copyright notice in the Description page of Project Settings.


#include "PEBaseGameplayAbility.h"

#include "CollisionDebugDrawingPublic.h"
#include "KismetTraceUtils.h"
#include "PEPlayerCharacter.h"
#include "PEPlayerController.h"

UPEBaseGameplayAbility::UPEBaseGameplayAbility(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UPEBaseGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (IsValid(AbilityCooldownComponentClass) && !IsValid(AbilityCooldownComponent))
	{
		AbilityCooldownComponent = NewObject<UPEAbilityCooldownComponent>(this, AbilityCooldownComponentClass);
	}
}

void UPEBaseGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                             const FGameplayEventData* TriggerEventData)
{
	// Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	//
	// UE_LOG(LogTemp, Warning, TEXT("%s: Server Executed Ability (1)"), *ActorInfo->OwnerActor->GetInstigatorController()->GetName())
	// APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetOwningActorFromActorInfo());
	// UCameraComponent* PlayerCamera = PC->CameraComponent;
	// FHitResult Hit;
	// GetWorld()->LineTraceSingleByChannel(Hit, PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentLocation() + PlayerCamera->GetForwardVector() * 2000, ECC_Pawn);
	// DrawDebugLine(GetWorld(), PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentLocation() + PlayerCamera->GetForwardVector() * 2000, FColor::Black);
	//
	// APEPlayerCharacter* HitActor = Cast<APEPlayerCharacter>(Hit.GetActor());
	// if (!HitActor)
	// {
	// 	return;
	// }
	// APEPlayerController* HitPlayerController = Cast<APEPlayerController>(HitActor->GetController());
	//
	// DrawDebugSphere(GetWorld(), Hit.Location, 15, 15,FColor::Black);
	// 	
	// if (HitActor && PC->GetController() != HitPlayerController)
	// {
	// 	UPEBaseCharacterAttributeSet* AttributeSet = Cast<UPEBaseCharacterAttributeSet>(HitActor->AttributeSet);
	// 	EDamageDirection DamageDirection = PC->DetermineDamageDirection(Hit);
	// 	float DamageMultipler = DetermineDamageMultiplier(DamageDirection, TODO);
	// 	AttributeSet->SetHealth(AttributeSet->GetHealth() - 1 * DamageMultipler);
	//
	// 	UPEHealthBarWidget* HealthBarWidget = Cast<UPEHealthBarWidget>(HitActor->HealthBarWidgetComponent->GetWidget());
	// 	if (!IsValid(HealthBarWidget))
	// 	{
	// 		return;
	// 	}
	// 	HealthBarWidget->UpdateHealthBar(HitActor->AttributeSet);
	// 	
	// 	UE_LOG(LogTemp, Warning, TEXT("%s; Health now %f, damaged from: %s, with multiplier: %f"), *HitActor->GetName(), AttributeSet->GetHealth(), *EDamageDirection_ToString(DamageDirection), DamageMultipler);
	// }
}

void UPEBaseGameplayAbility::DamageHealth(APEPlayerCharacter* HitActor, FHitResult Hit, float fAmount)
{
	// APEPlayerCharacter* HitActor = Cast<APEPlayerCharacter>(Hit.GetActor());
	UPEBaseCharacterAttributeSet* AttributeSet = Cast<UPEBaseCharacterAttributeSet>(HitActor->AttributeSet);
	
	EDamageDirection DamageDirection = HitActor->DetermineDamageDirection(Hit, FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
	float DamageMultipler = DetermineDamageMultiplier(DamageDirection, HitActor);
	AttributeSet->SetHealth(AttributeSet->GetHealth() - fAmount * DamageMultipler);
	
	UE_LOG(LogTemp, Warning, TEXT("%s; Health now %f, damaged from: %s, with multiplier: %f"), *HitActor->GetName(), AttributeSet->GetHealth(), *EDamageDirection_ToString(DamageDirection), DamageMultipler);
}

void UPEBaseGameplayAbility::HealHealth(APEPlayerCharacter* HitActor, FHitResult Hit, float fAmount)
{
	// APEPlayerCharacter* HitActor = Cast<APEPlayerCharacter>(Hit.GetActor());
	UPEBaseCharacterAttributeSet* AttributeSet = Cast<UPEBaseCharacterAttributeSet>(HitActor->AttributeSet);
	
	EDamageDirection DamageDirection = HitActor->DetermineDamageDirection(Hit, FLinearColor(0.0f, 1.0f, 0.0f, 1.0f));
	float DamageMultipler = DetermineDamageMultiplier(DamageDirection, HitActor);
	AttributeSet->SetHealth(AttributeSet->GetHealth() + fAmount * DamageMultipler);
	
	UE_LOG(LogTemp, Warning, TEXT("%s; Health now %f, damaged from: %s, with multiplier: %f"), *HitActor->GetName(), AttributeSet->GetHealth(), *EDamageDirection_ToString(DamageDirection), DamageMultipler);
}

 APEPlayerCharacter* UPEBaseGameplayAbility::LineTraceAbility(const APEPlayerCharacter* PC, const FGameplayAbilityActorInfo* ActorInfo, float fRange, FColor Color)
{
	UCameraComponent* PlayerCamera = PC->CameraComponent;
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentLocation() + PlayerCamera->GetForwardVector() * fRange, ECC_Pawn);
	DrawDebugLine(GetWorld(), PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentLocation() + PlayerCamera->GetForwardVector() * fRange, Color);
	
	APEPlayerCharacter* HitActor = Cast<APEPlayerCharacter>(Hit.GetActor());
	if (!HitActor)
	{
		return nullptr;
	}
	APEPlayerController* HitPlayerController = Cast<APEPlayerController>(HitActor->GetController());

	DrawDebugSphere(GetWorld(), Hit.Location, 15, 15, Color);
		
	if (HitActor && PC->GetController() != HitPlayerController)
	{
		return HitActor;
	}

	return nullptr;
}

float UPEBaseGameplayAbility::DetermineDamageMultiplier(const EDamageDirection DamageDirection, APEPlayerCharacter* PC) const
{
	if (!IsValid(PC))
	{
		UE_LOG(LogTemp, Warning, TEXT("DetermineDamageMultiplier, Invalid PC."));
		return 0.0f;
	}
	switch (DamageDirection)
	{
		case EDamageDirection::Front:
			return PC->AttributeSet->GetDamageDirectionFront();
		case EDamageDirection::Side:
			return PC->AttributeSet->GetDamageDirectionSide();
		case EDamageDirection::Back:
			return PC->AttributeSet->GetDamageDirectionBack();
		default:
			UE_LOG(LogTemp, Warning, TEXT("Something went wrong, was not able to determine a damage direction."));
			return 0.0f;
	}
}
