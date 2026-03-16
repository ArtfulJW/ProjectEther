// Fill out your copyright notice in the Description page of Project Settings.

#include "PEChainedCalculation.h"

#include "BoneWeights.h"
#include "PEPlayerCharacter.h"
#include "PEPlayerController.h"
#include "PE_GEChained.h"
#include "Engine/SpringInterpolator.h"


void UPEChainedCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                   FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* TargetActor = TargetAbilitySystemComponent->GetAvatarActor();
	APEPlayerCharacter* TargetPlayerCharacter = Cast<APEPlayerCharacter>(TargetActor);
	APEPlayerController* TargetPlayerController = Cast<APEPlayerController>(TargetActor->GetInstigatorController());
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
	
	UE_LOG(LogTemp, Warning, TEXT("Chained GameplayAbility Calculation"));

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayEffectContextHandle Context = Spec.GetContext();
	const UGameplayAbility* GameplayAbility = Context.GetAbility();

	const FGameplayTagContainer* SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();
	
	
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTag;
	EvaluationParameters.TargetTags = TargetTag;
	
	FVector TargetPCPosition = Spec.GetContext().GetOrigin();

	FRK4SpringInterpolator<FVector> ChainSpringInterpolator;
	ChainSpringInterpolator.SetDefaultSpringConstants(SpringStiffness, SpringDamping);
	
	TargetPlayerCharacter->ChainPlayer(TargetPCPosition, Cast<UPE_GEChained>(Spec.Def.Get())->fDuration, ChainSpringInterpolator);
}