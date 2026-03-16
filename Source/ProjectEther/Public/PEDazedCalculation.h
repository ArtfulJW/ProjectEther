// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "PEDazedCalculation.generated.h"

class APEPlayerCharacter;
/**
 * 
 */
UCLASS()
class PROJECTETHER_API UPEDazedCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};