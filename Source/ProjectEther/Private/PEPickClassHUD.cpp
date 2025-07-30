// Fill out your copyright notice in the Description page of Project Settings.


#include "PEPickClassHUD.h"

#include "PEGameState.h"
#include "Kismet/GameplayStatics.h"

void UPEPickClassHUD::NativeConstruct()
{
	Super::NativeConstruct();

	BerserkerClassButton->OnClicked.AddDynamic(this, &UPEPickClassHUD::RequestSpawnPlayerCharacter);
	MageClassButton->OnClicked.AddDynamic(this, &UPEPickClassHUD::RequestSpawnPlayerCharacter);
	PriestClassButton->OnClicked.AddDynamic(this, &UPEPickClassHUD::RequestSpawnPlayerCharacter);
}

void UPEPickClassHUD::RequestSpawnPlayerCharacter_Implementation()
{
	APEPlayerCharacter* PlayerCharacter = Cast<APEPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	APEPlayerController* PlayerController = Cast<APEPlayerController>(PlayerCharacter->GetController());
	APEGameState* GameState = Cast<APEGameState>(GetWorld()->GetGameState());
	GameState->ServerSpawnPlayerCharacter(PlayerController, PlayerController->CharacterClass);
}