// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityContainer.generated.h"

class UGameplayAbility;
/**
 * 
 */
UCLASS(Blueprintable)
class PROJECTETHER_API UAbilityContainer : public UObject
{
	GENERATED_BODY()
public:
	UAbilityContainer();
	
	virtual ~UAbilityContainer() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability Container Specs")
	int AbilityCost = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability Container Specs")
	UAnimMontage* CharacterAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability Container Specs")
	UAnimMontage* FPSAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability Container Specs")
	TSubclassOf<UGameplayAbility> Ability;
};