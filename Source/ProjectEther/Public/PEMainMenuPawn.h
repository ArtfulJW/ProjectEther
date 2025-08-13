// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEMainMenuHUD.h"
#include "Components/Button.h"
#include "GameFramework/Actor.h"
#include "PEMainMenuPawn.generated.h"

UCLASS()
class PROJECTETHER_API APEMainMenuPawn : public APawn
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Sets default values for this actor's properties
	APEMainMenuPawn();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Main Menu HUD")
	TSubclassOf<UPEMainMenuHUD> MainMenuHUDClass;

	UPROPERTY()
	UPEMainMenuHUD* MainMenuHUD;
};