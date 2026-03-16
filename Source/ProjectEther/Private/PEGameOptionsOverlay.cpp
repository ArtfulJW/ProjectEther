// Fill out your copyright notice in the Description page of Project Settings.


#include "PEGameOptionsOverlay.h"

#include "PEPlayerController.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"

void UPEGameOptionsOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	SaveSettingsButton->OnClicked.AddDynamic(this, &UPEGameOptionsOverlay::SaveSettings);
}

void UPEGameOptionsOverlay::SaveSettings()
{
	Scalability::FQualityLevels Levels;
	int QualityLevel = FMath::Clamp(SaveSettingsSlider->GetValue(),0,3);
	Levels.SetFromSingleQualityLevel(QualityLevel);
	Scalability::SetQualityLevels(Levels);
	Scalability::SaveState(GGameUserSettingsIni);

	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();
	if (IsValid(UserSettings))
	{
		UserSettings->SetOverallScalabilityLevel(QualityLevel);
		UserSettings->ApplySettings(false);
		UserSettings->SaveSettings();
	}

	IConsoleManager& ConsoleManager = IConsoleManager::Get();
	ConsoleManager.FindConsoleVariable(TEXT("sg.AntiAliasingQuality"))->Set(QualityLevel);
	ConsoleManager.FindConsoleVariable(TEXT("sg.ShadowQuality"))->Set(QualityLevel);
	ConsoleManager.FindConsoleVariable(TEXT("sg.GlobalIlluminationQuality"))->Set(QualityLevel);
	ConsoleManager.FindConsoleVariable(TEXT("sg.ReflectionQuality"))->Set(QualityLevel);
	ConsoleManager.FindConsoleVariable(TEXT("sg.ViewDistanceQuality"))->Set(QualityLevel);
	ConsoleManager.FindConsoleVariable(TEXT("sg.PostProcessQuality"))->Set(QualityLevel);
	ConsoleManager.FindConsoleVariable(TEXT("sg.TextureQuality"))->Set(QualityLevel);
	ConsoleManager.FindConsoleVariable(TEXT("sg.EffectsQuality"))->Set(QualityLevel);
	ConsoleManager.FindConsoleVariable(TEXT("sg.FoliageQuality"))->Set(QualityLevel);
	ConsoleManager.FindConsoleVariable(TEXT("sg.LandscapeQuality"))->Set(QualityLevel);
}