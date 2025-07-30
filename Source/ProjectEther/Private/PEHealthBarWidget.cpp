// Fill out your copyright notice in the Description page of Project Settings.

#include "PEPlayerCharacter.h"
#include "PEHealthBarWidget.h"

void UPEHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(PlayerCharacterRef))
	{
		// HealthBar->AddBinding(&UPEHealthBarWidget::ServerUpdateHealthBar, PlayerCharacterRef,);
		// HealthBar.Binding
	}
}

void UPEHealthBarWidget::ServerUpdateHealthBar_Implementation(float fPercentage)
{
	if (fPercentage > 1.0f)
	{
		FMath::Max(fPercentage, 1.0f);
	}

	if (fPercentage < 0.0f)
	{
		FMath::Min(fPercentage, 0.0f);
	}

	if (fPercentage == HealthBar->GetPercent())
	{
		return;
	}
	
	HealthBar->SetPercent(fPercentage);
}