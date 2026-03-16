// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Engine/SpringInterpolator.h"
#include "PE_GEChained.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTETHER_API UPE_GEChained : public UGameplayEffect
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Spec")
	float fDuration = 3.0f;
};