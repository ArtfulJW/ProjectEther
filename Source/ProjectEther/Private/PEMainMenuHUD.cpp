// Fill out your copyright notice in the Description page of Project Settings.

#include "PEMainMenuHUD.h"

#include "Kismet/GameplayStatics.h"

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
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}
	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
	if (!IsValid(PlayerController))
	{
		return;
	}

	// PlayerController->ClientTravel(FString("/Script/Engine.World'/Game/Maps/MinimalMap.MinimalMap'"),TRAVEL_Absolute);
	
	// PlayerController->ClientStartOnlineSession();
	// UGameplayStatics::OpenLevel(World, FName("MinimalMap.MinimalMap"), true, FString("?listen"));

	// World->ServerTravel(FString("/Script/Engine.World'/Game/Maps/MinimalMap.MinimalMap'"), true);

	ServerRequestTravel();
	
	UE_LOG(LogTemp, Display, TEXT("Traveling to Main Game"));
}

void UPEMainMenuHUD::ShowLoadoutScreen()
{
	UE_LOG(LogTemp, Display, TEXT("Showing Loadout"));
}

void UPEMainMenuHUD::ServerRequestTravel_Implementation()
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}
	
	// World->ServerTravel(FString("/Script/Engine.World'/Game/Maps/MinimalMap.MinimalMap'"), true);

	World->Exec(GetWorld(), TEXT("servertravel MinimalMap"));
}
