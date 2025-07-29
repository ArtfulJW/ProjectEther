// Fill out your copyright notice in the Description page of Project Settings.

#include "PEGameMode.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystemUtils.h"

APEGameMode::APEGameMode():
NumEtherToWin(3)
{
	bUseSeamlessTravel = true;

	// ServerStartSession();
}

void APEGameMode::ServerStartSession_Implementation()
{
	IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld());
	IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();

	FOnlineSessionSettings SessionSettings;
	SessionSettings.NumPublicConnections = 6;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;

	// UGameInstance* GameInstance = GetGameInstance();

	// FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	// CreateSessionCompleteDelegate.BindUObject();

	SessionInterface->CreateSession(0, FName("123"), SessionSettings);
}

// APlayerController* APEGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal,
// 	const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
// {
// 	Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
// 	
// 	APEPlayerController* APEPlayerController = PEPlayerControllerClass.GetDefaultObject();
// 	// NewPlayer->SwitchController(PEPlayerControllerClass.GetDefaultObject());
// 	
// 	return APEPlayerController;
// }

int APEGameMode::GetNumEtherToWin() const
{
	return NumEtherToWin;
}
