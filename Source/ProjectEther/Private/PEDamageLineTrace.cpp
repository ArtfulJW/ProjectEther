// Fill out your copyright notice in the Description page of Project Settings.


#include "PEDamageLineTrace.h"

#include "PEPlayerController.h"

void UPEDamageLineTrace::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Warning, TEXT("%s: Server Executed Ability DamageLineTrace"), *ActorInfo->OwnerActor->GetInstigatorController()->GetName())
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetOwningActorFromActorInfo());
	UCameraComponent* PlayerCamera = PC->CameraComponent;
	FHitResult Hit;

	float Range = 500.0f;
	float fDamage = 5.0f;
	if (Cast<APEPlayerController>(PC->GetController())->CharacterClass == EClassType::Mage)
	{
		Range = 3000.0f;
		fDamage = 7.0f;
	}
	
	GetWorld()->LineTraceSingleByChannel(Hit, PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentLocation() + PlayerCamera->GetForwardVector() * Range, ECC_Pawn);
	DrawDebugLine(GetWorld(), PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentLocation() + PlayerCamera->GetForwardVector() * Range, FColor::Red);

	APEPlayerCharacter* HitActor = Cast<APEPlayerCharacter>(Hit.GetActor());
	if (!HitActor)
	{
		return;
	}
	APEPlayerController* HitPlayerController = Cast<APEPlayerController>(HitActor->GetController());
	DrawDebugSphere(GetWorld(), Hit.Location, 15, 15,FColor::Red);

	if (HitActor && PC->GetController() != HitPlayerController)
	{
		DamageHealth(HitActor, Hit, fDamage);

		UPEHealthBarWidget* HealthBarWidget = Cast<UPEHealthBarWidget>(HitActor->HealthBarWidgetComponent->GetWidget());
		if (!IsValid(HealthBarWidget))
		{
			return;
		}
		HealthBarWidget->UpdateHealthBar(HitActor->AttributeSet);
	}
}
