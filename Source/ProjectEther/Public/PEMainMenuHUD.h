// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "PEMainMenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTETHER_API UPEMainMenuHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UPEMainMenuHUD(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Main Menu HUD")
	TObjectPtr<UButton> LoadoutButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Main Menu HUD")
	TObjectPtr<UButton> PlayButton;
	
	UFUNCTION()
	void TravelToMainGame();

	UFUNCTION()
	void ShowLoadoutScreen();
};