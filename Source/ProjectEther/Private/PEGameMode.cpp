// Fill out your copyright notice in the Description page of Project Settings.

#include "PEGameMode.h"

#include "PEPlayerController.h"

APEGameMode::APEGameMode():
NumEtherToWin(3)
{
	bUseSeamlessTravel = true;
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
