// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "PE_GEDazed.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTETHER_API UPE_GEDazed : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Spec")
	float fDuration = 5.0f;
};