// Fill out your copyright notice in the Description page of Project Settings.


#include "PEDazedCalculation.h"

#include "PEBlindside.h"
#include "PEPlayerCharacter.h"
#include "PEPlayerController.h"
#include "PE_GEDazed.h"

void UPEDazedCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                 FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

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
	
	UE_LOG(LogTemp, Warning, TEXT("Dazed GameplayAbility Calculation"));

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayEffectContextHandle Context = Spec.GetContext();
	const UGameplayAbility* GameplayAbility = Context.GetAbility();

	const FGameplayTagContainer* SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();
	
	
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTag;
	EvaluationParameters.TargetTags = TargetTag;

	APEPlayerController* TargetPlayerController = Cast<APEPlayerController>(TargetPlayerCharacter->GetController());
	TargetPlayerController->bCanBasicAttack = false;
	
	FTimerDelegate TimerDelegate;
	FTimerHandle TimerHandle;
	TimerDelegate.BindUFunction(SourcePlayerCharacter, FName("AllowBasicAttack"),TargetPlayerCharacter);

	// GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, Spec.Duration, false);

	UWorld* World = TargetPlayerCharacter->GetWorld();
	World->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, Cast<UPE_GEDazed>(Spec.Def.Get())->fDuration, false);

	TargetPlayerCharacter->AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(Spec.Def.GetClass(), SourcePlayerCharacter->AbilitySystemComponent, -1);
}
