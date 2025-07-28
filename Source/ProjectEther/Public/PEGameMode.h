// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PEGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTETHER_API APEGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APEGameMode();

	UFUNCTION(BlueprintCallable, Category="Ether War Function")
	int GetNumEtherToWin() const;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ether War Specifications", meta=(AllowPrivateAccess=true))
	int NumEtherToWin;
};