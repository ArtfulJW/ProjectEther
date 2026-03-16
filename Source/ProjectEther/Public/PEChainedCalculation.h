// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectCalculation.h"
#include "GameplayEffectExecutionCalculation.h"
#include "PEChainedCalculation.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTETHER_API UPEChainedCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	// mutable FTimerHandle TimerHandle;
	// mutable bool bLoop = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability Specs")
	float SpringStiffness = 250.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability Specs")
	float SpringDamping = 0.1f;
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};