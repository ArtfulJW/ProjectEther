// Fill out your copyright notice in the Description page of Project Settings.

#include "PEHealthBarWidget.h"

#include "PEBaseCharacterAttributeSet.h"

void UPEHealthBarWidget::UpdateHealthBar(UPEBaseCharacterAttributeSet* AttributeSet)
{
	float fPercentage = AttributeSet->GetHealth() / 100;
	HealthBar->SetPercent(fPercentage);
}