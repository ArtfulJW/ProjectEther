// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PEGameOptionsOverlay.generated.h"

class USlider;
class UButton;
/**
 * 
 */
UCLASS()
class PROJECTETHER_API UPEGameOptionsOverlay : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spec")
    TObjectPtr<UButton> SaveSettingsButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spec")
	TObjectPtr<USlider> SaveSettingsSlider;
	
	UFUNCTION()
	void SaveSettings();
};
