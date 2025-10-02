// Fill out your copyright notice in the Description page of Project Settings.

#include "PEHolyPresenceCalculation.h"
#include "AbilitySystemComponent.h"
#include "PEHealingLineTrace.h"
#include "PEPlayerCharacter.h"

// struct AttributeCapture
// {
// 	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
// 	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealth);
//
// 	AttributeCapture()
// 	{
// 		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet, Health, Target, false)
// 	}
// };

void UPEHolyPresenceCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* TargetActor = TargetAbilitySystemComponent->GetAvatarActor();
	APEPlayerCharacter* TargetPlayerCharacter = Cast<APEPlayerCharacter>(TargetActor);
	UPEBaseCharacterAttributeSet* TargetPlayerAttributeSet = TargetPlayerCharacter->AttributeSet;
	if (!IsValid(TargetPlayerCharacter))
	{
		return;
	}

	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();
	AActor* SourceActor = SourceAbilitySystemComponent->GetAvatarActor();
	APEPlayerCharacter* SourcePlayerCharacter = Cast<APEPlayerCharacter>(SourceActor);
	if (!IsValid(SourcePlayerCharacter))
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("HolyPresence GameplayAbility Calculation"))

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();
	
	
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTag;
	EvaluationParameters.TargetTags = TargetTag;

	FGameplayEffectAttributeCaptureDefinition* CaptureDef = new FGameplayEffectAttributeCaptureDefinition(
		TargetPlayerAttributeSet->GetHealthAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);

	float OutMagnitude = TargetPlayerAttributeSet->GetHealth() + 3.0f;

	TargetPlayerCharacter->AttributeSet->SetHealth(TargetPlayerCharacter->AttributeSet->GetHealth() + 3.0f);
	
	// ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(*CaptureDef, EvaluationParameters, OutMagnitude);
	
	// Update Healthbar widget
	// UPEHealthBarWidget* HealthBarWidget = Cast<UPEHealthBarWidget>(TargetPlayerCharacter->HealthBarWidgetComponent->GetWidget());
	// if (!IsValid(HealthBarWidget))
	// {
	// 	return;
	// }
	// HealthBarWidget->ServerUpdateHealthBar(TargetPlayerCharacter->AttributeSet);
}
