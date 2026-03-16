// Fill out your copyright notice in the Description page of Project Settings.


#include "PEPickClassHUD.h"

#include "PEGameState.h"
#include "GameFramework/SpectatorPawn.h"
#include "Kismet/GameplayStatics.h"

void UPEPickClassHUD::NativeConstruct()
{
	Super::NativeConstruct();

	BerserkerClassButton->OnClicked.AddDynamic(this, &UPEPickClassHUD::SetBerserkerClassAndRequestSpawn);
	MageClassButton->OnClicked.AddDynamic(this, &UPEPickClassHUD::SetMageClassAndRequestSpawn);
	PriestClassButton->OnClicked.AddDynamic(this, &UPEPickClassHUD::SetPriestClassAndRequestSpawn);
	SpectatorPawnButton->OnClicked.AddDynamic(this, &UPEPickClassHUD::SelectSpectatorPawnAndRequestSpawn);
}

void UPEPickClassHUD::SetBerserkerClassAndRequestSpawn()
{
	const ULocalPlayer* LocalPlayer = GetOwningLocalPlayer();
	if (!IsValid(LocalPlayer))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid LocalPlayer"));
		return;
	}
	
	APEPlayerController* PlayerController = Cast<APEPlayerController>(GetOwningPlayer());
	PlayerController->ServerSetCharacterClass(PlayerController, EClassType::Berserker);
	PlayerController->RequestTeamAssignment();
	PlayerController->ClientSetupInputControls();
	PlayerController->RequestSpawn();
}

void UPEPickClassHUD::SetMageClassAndRequestSpawn()
{
	APEPlayerController* PlayerController = Cast<APEPlayerController>(GetOwningPlayer());
	PlayerController->ServerSetCharacterClass(PlayerController, EClassType::Mage);
	PlayerController->RequestTeamAssignment();
	PlayerController->ClientSetupInputControls();
	PlayerController->RequestSpawn();
}

void UPEPickClassHUD::SetPriestClassAndRequestSpawn()
{
	APEPlayerController* PlayerController = Cast<APEPlayerController>(GetOwningPlayer());
	PlayerController->ServerSetCharacterClass(PlayerController, EClassType::Priest);
	PlayerController->RequestTeamAssignment();
	PlayerController->ClientSetupInputControls();
	PlayerController->RequestSpawn();
}

void UPEPickClassHUD::SelectSpectatorPawnAndRequestSpawn()
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}
	
	APEPlayerController* PlayerController = Cast<APEPlayerController>(GetOwningPlayer());
	if (!IsValid(PlayerController))
	{
		return;
	}
	
	PlayerController->ServerRequestSpectatorSpawn();
}