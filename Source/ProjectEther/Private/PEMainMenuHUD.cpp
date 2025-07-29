// Fill out your copyright notice in the Description page of Project Settings.

#include "PEMainMenuHUD.h"

UPEMainMenuHUD::UPEMainMenuHUD(const FObjectInitializer& ObjectInitializer): UUserWidget(ObjectInitializer)
{
}

void UPEMainMenuHUD::NativeConstruct()
{
	Super::NativeConstruct();

	LoadoutButton->OnClicked.AddDynamic(this, &UPEMainMenuHUD::ShowLoadoutScreen);
	PlayButton->OnClicked.AddDynamic(this, &UPEMainMenuHUD::TravelToMainGame);
}

void UPEMainMenuHUD::TravelToMainGame()
{
	UE_LOG(LogTemp, Display, TEXT("Traveling to Main Game"));
}

void UPEMainMenuHUD::ShowLoadoutScreen()
{
	UE_LOG(LogTemp, Display, TEXT("Showing Loadout"));
}
