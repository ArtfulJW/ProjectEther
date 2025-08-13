// Fill out your copyright notice in the Description page of Project Settings.

#include "PEMainMenuPawn.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
APEMainMenuPawn::APEMainMenuPawn():
	MainMenuHUD(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APEMainMenuPawn::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	if (GetNetMode() >= NM_Client)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
		MainMenuHUD = CreateWidget<UPEMainMenuHUD>(PlayerController, MainMenuHUDClass);
		if (MainMenuHUD)
		{
			MainMenuHUD->AddToViewport();
		}
		PlayerController->SetShowMouseCursor(true);
	}
}

// Called every frame
void APEMainMenuPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}