// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "PEHealthBarWidget.generated.h"

class UPEBaseCharacterAttributeSet;
class APEPlayerCharacter;
/**
 * 
 */
UCLASS()
class PROJECTETHER_API UPEHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Bar Specs")
	UProgressBar* HealthBar;

	UFUNCTION(BlueprintCallable, Category = "Health Bar Functions")
	void UpdateHealthBar(UPEBaseCharacterAttributeSet* AttributeSet);
};