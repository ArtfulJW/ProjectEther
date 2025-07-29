// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEPlayerController.h"
#include "Online.h"
#include "Online/CoreOnline.h"
#include "GameFramework/GameModeBase.h"
#include "PEGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTETHER_API APEGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ether War Specifications", meta=(AllowPrivateAccess=true))
	int NumEtherToWin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ether War Specifications", meta=(AllowPrivateAccess=true))
	TSubclassOf<APEPlayerController> PEPlayerControllerClass;
	
public:
	APEGameMode();

	// virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal,
	// const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	// UPROPERTY(VisibleAnywhere, Category="APEGameMode Session Specs")
	// AGameSession* PEGameSession;

	UFUNCTION(Server, Reliable)
	void ServerStartSession();
	
	UFUNCTION(BlueprintCallable, Category="Ether War Function")
	int GetNumEtherToWin() const;
};