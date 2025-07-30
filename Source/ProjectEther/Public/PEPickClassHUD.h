// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "PEPickClassHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTETHER_API UPEPickClassHUD : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pick Class HUD")
	TObjectPtr<UButton> BerserkerClassButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pick Class HUD")
	TObjectPtr<UButton> MageClassButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pick Class HUD")
	TObjectPtr<UButton> PriestClassButton;

	UFUNCTION(Server, Reliable)
	void RequestSpawnPlayerCharacter();
};