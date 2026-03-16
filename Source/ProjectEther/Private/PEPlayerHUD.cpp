// Fill out your copyright notice in the Description page of Project Settings.

#include "PEPlayerHUD.h"

#include "PEGameState.h"
#include "PEMathLibrary.h"
#include "PEPlayerCharacter.h"
#include "PEPlayerController.h"
#include "Kismet/GameplayStatics.h"

void UPEPlayerHUD::ActivateHitMarker()
{
	if (!IsValid(HitMarker))
	{
		return;
	}

	bIsLerpingHitMarker = true;
	HitMarker->SetOpacity(2.0f);
}

void UPEPlayerHUD::LerpHitMarkerAlpha()
{
	if (!IsValid(HitMarker))
	{
		return;
	}

	float fAlpha = HitMarker->GetColorAndOpacity().A;
	
	if (bIsLerpingHitMarker)
	{
		fAlpha = FMath::Lerp<float>(fAlpha, 0.0f, 0.5f);
		HitMarker->SetOpacity(fAlpha);

		if (fAlpha <= 0.0f)
		{
			bIsLerpingHitMarker = false;
		}
	}
}

void UPEPlayerHUD::SetDamageVignetteAlpha()
{
	if (!IsValid(DamageVignette))
	{
		return;
	}
	
	APEPlayerCharacter* PlayerCharacter = Cast<APEPlayerCharacter>(GetOwningPlayerPawn());
	if (!IsValid(PlayerCharacter))
	{
		return;
	}

	float CurrentHealth = PlayerCharacter->AttributeSet->GetHealth();
	float MaximumHealth = PlayerCharacter->AttributeSet->GetMaxHealth();
	float IntermediateFloat = 1 - (CurrentHealth / MaximumHealth);
	
	DamageVignette->SetOpacity(IntermediateFloat);
}

void UPEPlayerHUD::UpdateHUDHealthBar(const UPEBaseCharacterAttributeSet* InAttributeSet) const
{
	if (!IsValid(HealthBar))
	{
		return;
	}
	
	if (!IsValid(InAttributeSet))
	{
		return;
	}

	APEPlayerCharacter* PlayerCharacter = Cast<APEPlayerCharacter>(InAttributeSet->GetOwningActor());
	if (!IsValid(PlayerCharacter))
	{
		return;
	}

	float fPercentage = InAttributeSet->GetHealth() / PlayerCharacter->fMaxHealthMaxValue;
	
	HealthBar->SetPercent(fPercentage);
}

void UPEPlayerHUD::SetHUDTeamTag(FText InText)
{
	if (!IsValid(HUDTeamTagTextBlock))
	{
		return;
	}
	HUDTeamTagTextBlock->SetText(InText);
}

void UPEPlayerHUD::UpdateScoreBoard()
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}
	
	APEGameState* GameState = Cast<APEGameState>(UGameplayStatics::GetGameState(World));
	if (!IsValid(GameState))
	{
		return;
	}
	
	TeamOnePointsTextBlock->SetText(FText::FromString(FString::FromInt(GameState->TeamOneNumberEtherDeposited)));
	TeamTwoPointsTextBlock->SetText(FText::FromString(FString::FromInt(GameState->TeamTwoNumberEtherDeposited)));
	
	float fEtherDistanceToGoal = 0.0f;
	float TotalDistance = 0.0f;
	ETeam OutTeam = GameState->DetermineEtherClosestTeam(fEtherDistanceToGoal, TotalDistance);

	if (fEtherDistanceToGoal == 0.0f || TotalDistance == 0.0f)
	{
		return;
	}

	float OutPercent = 1 - (fEtherDistanceToGoal / TotalDistance);
	
	switch (OutTeam)
	{
		case ETeam::TeamOne:
			TeamOneDistanceProgressBar->SetPercent(OutPercent);
			TeamTwoDistanceProgressBar->SetPercent(0.0f);
			// Set Color here
			return;
		case ETeam::TeamTwo:
			TeamOneDistanceProgressBar->SetPercent(0.0f);
			TeamTwoDistanceProgressBar->SetPercent(OutPercent);
			// Set Color here
			return;
		case ETeam::NoTeam:
			TeamOneDistanceProgressBar->SetPercent(0.0f);
			TeamTwoDistanceProgressBar->SetPercent(0.0f);
			// Set Color here
			return;
	}
}

void UPEPlayerHUD::ActivateHitIndicator(EDirectionDamageIndicator InDirection, FLinearColor InColor)
{
	switch (InDirection)
	{
	case EDirectionDamageIndicator::Front:
		HitIndicatorFront->SetOpacity(1);
		HitIndicatorFront->SetBrushTintColor(FSlateColor(InColor));
		bLerpFrontHitIndicator = true;
		return;
	case EDirectionDamageIndicator::Left:
		HitIndicatorLeft->SetOpacity(1);
		HitIndicatorLeft->SetBrushTintColor(FSlateColor(InColor));
		bLerpLeftHitIndicator = true;
		return;
	case EDirectionDamageIndicator::Right:
		HitIndicatorRight->SetOpacity(1);
		HitIndicatorRight->SetBrushTintColor(FSlateColor(InColor));
		bLerpRightHitIndicator = true;
		return;
	case EDirectionDamageIndicator::Back:
		HitIndicatorBack->SetOpacity(1);
		HitIndicatorBack->SetBrushTintColor(FSlateColor(InColor));
		bLerpBackHitIndicator = true;
		return;
	}
}

void UPEPlayerHUD::UpdateHitIndicators()
{
	if (bLerpFrontHitIndicator)
	{
		float OutAlpha = LerpHitIndicatorAlpha(HitIndicatorFront, fHitIndicatorFadeSpeed);

		if (OutAlpha <= 0.0f)
		{
			bLerpFrontHitIndicator = false;
		}
		HitIndicatorFront->SetOpacity(OutAlpha);
	}
	
	if (bLerpLeftHitIndicator)
	{
		float OutAlpha = LerpHitIndicatorAlpha(HitIndicatorLeft, fHitIndicatorFadeSpeed);
		if (OutAlpha <= 0.0f)
		{
			bLerpLeftHitIndicator = false;
		}
		HitIndicatorLeft->SetOpacity(OutAlpha);
	}

	if (bLerpRightHitIndicator)
	{
		float OutAlpha = LerpHitIndicatorAlpha(HitIndicatorRight, fHitIndicatorFadeSpeed);
		if (OutAlpha <= 0.0f)
		{
			bLerpRightHitIndicator = false;
		}
		HitIndicatorRight->SetOpacity(OutAlpha);
	}

	if (bLerpBackHitIndicator)
	{
		float OutAlpha = LerpHitIndicatorAlpha(HitIndicatorBack, fHitIndicatorFadeSpeed);
		if (OutAlpha <= 0.0f)
		{
			bLerpBackHitIndicator = false;
		}
		HitIndicatorBack->SetOpacity(OutAlpha);
	}
}

float UPEPlayerHUD::LerpHitIndicatorAlpha(const UImage* InHitIndicator, float delta)
{
	if (IsValid(InHitIndicator))
	{
		float fAlpha = InHitIndicator->GetColorAndOpacity().A;
		return FMath::Lerp<float>(fAlpha, 0.0f, delta);	
	}
	return 0.0f;
}

void UPEPlayerHUD::UpdateGenericResourceBar(float InPercent)
{
	ResourceBar->SetPercent(InPercent);
}

void UPEPlayerHUD::UpdateResourceTexts(const UPEBaseCharacterAttributeSet* InAttributeSet)
{
	if (!IsValid(InAttributeSet))
	{
		return;
	}
	
	ResourceText->SetText(FText::AsNumber(InAttributeSet->GetResource()));
	ResourceMaxText->SetText(FText::AsNumber(InAttributeSet->GetResourceMaxValue()));
}

void UPEPlayerHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	LerpHitMarkerAlpha();
	SetDamageVignetteAlpha();
	UpdateScoreBoard();
	UpdateHitIndicators();
}
