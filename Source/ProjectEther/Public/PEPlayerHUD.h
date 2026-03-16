// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEBaseCharacterAttributeSet.h"
#include "PEEtherWarStructs.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "PEPlayerHUD.generated.h"

class APEPlayerController;
class APEPlayerCharacter;
/* 
 * 
 */
UCLASS()
class PROJECTETHER_API UPEPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD Specs")
	UImage* HitMarker = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD Specs")
	UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD Specs")
	UProgressBar* ResourceBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD Specs")
	UImage* DamageVignette = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD Specs")
	UTextBlock* HUDTeamTagTextBlock = nullptr;
	
	bool bIsLerpingHitMarker = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD Specs")
	UTextBlock* TeamOnePointsTextBlock = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD Specs")
	UTextBlock* TeamTwoPointsTextBlock = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD Specs")
	UProgressBar* TeamOneDistanceProgressBar = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD Specs")
	UProgressBar* TeamTwoDistanceProgressBar = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD Specs")
	UImage* HitIndicatorFront;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD Specs")
	UImage* HitIndicatorRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD Specs")
	UImage* HitIndicatorLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD Specs")
	UImage* HitIndicatorBack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD Specs")
	UTextBlock* ResourceText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD Specs")
	UTextBlock* ResourceMaxText = nullptr;

	bool bLerpFrontHitIndicator = false;
	bool bLerpLeftHitIndicator = false;
	bool bLerpRightHitIndicator = false;
	bool bLerpBackHitIndicator = false;

	float fHitIndicatorFadeSpeed = 0.25f;
	
	void ActivateHitMarker();
	
	void LerpHitMarkerAlpha();

	void SetDamageVignetteAlpha();

	void UpdateHUDHealthBar(const UPEBaseCharacterAttributeSet* InAttributeSet) const;

	void SetHUDTeamTag(FText InText);

	void UpdateScoreBoard();

	// UFUNCTION(Server, Reliable)
	// void ServerActivateHitIndicator(APEPlayerController* HitCharacter, EDirectionDamageIndicator InDirection);
	
	void ActivateHitIndicator(EDirectionDamageIndicator InDirection, FLinearColor InColor);
	
	void UpdateHitIndicators();

	float LerpHitIndicatorAlpha(const UImage* InHitIndicator, float delta);

	void UpdateGenericResourceBar(float InPercent);

	void UpdateResourceTexts(const UPEBaseCharacterAttributeSet* InAttributeSet);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};