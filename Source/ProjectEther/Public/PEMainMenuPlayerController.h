// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEMainMenuHUD.h"
#include "GameFramework/PlayerController.h"
#include "PEMainMenuPlayerController.generated.h"

class UPEPickClassHUD;
/**
 * 
 */
UCLASS()
class PROJECTETHER_API APEMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD")
	TSubclassOf<UPEMainMenuHUD> PEMainMenuHUDClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="HUD")
	UUserWidget* PEMainMenuClassHUD;
	
	virtual void BeginPlay() override;
};