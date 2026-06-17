// Fill out your copyright notice in the Description page of Project Settings.


#include "PEMainMenuPlayerController.h"


void APEMainMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		PEMainMenuClassHUD = CreateWidget<UPEMainMenuHUD>(this, PEMainMenuHUDClass);
		PEMainMenuClassHUD->AddToViewport();
		SetShowMouseCursor(true);
	}
}
