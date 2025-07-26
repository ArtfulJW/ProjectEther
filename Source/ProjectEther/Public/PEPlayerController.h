// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayAbilitySpecHandle.h"
#include "InputMappingContext.h"
#include "PEInteractableBase.h"
#include "PEEtherWarStructs.h"
#include "PEPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTETHER_API APEPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	/*
	 * 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputMappingContext* InputMapping;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* AbilityAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* WeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* DeployInteractableAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spectator")
	TSubclassOf<ASpectatorPawn> PESpectatorPawn;

	UPROPERTY(Replicated)
	TEnumAsByte<ETeam> Team;
	
	void MoveEvent(const FInputActionValue& Value);

	UFUNCTION(Server, Unreliable)
	void ServerMovePlayer(APEPlayerController* Requester, FVector InVector);
	
	void LookEvent(const FInputActionValue& Value);

	UFUNCTION(Server, Unreliable)
	void ServerLookEvent(APEPlayerController* Requester, FRotator InRotator);
	
	void UseAbilityEvent(const FInputActionValue& Value);

	UFUNCTION(Server, Unreliable)
	void ServerUseGameplayAbilityEvent(APEPlayerController* Requester, FGameplayAbilitySpecHandle AbilityHandle);

	void UseWeaponEvent(const FInputActionValue& Value);

	bool IsPossessingSpectatorPawn(APEPlayerController* Requester);
	
	void InteractEvent();

	void DeployInteractableEvent();

	UFUNCTION(Server, Reliable)
	void ServerInteractEvent(APEPlayerController* Requester, AActor* InActor);

	UFUNCTION(Server, Reliable)
	void ServerDropInteractableActor(APEPlayerController* Requester);

	UFUNCTION(Server, Reliable)
	void ServerDeployInteractable(APEInteractableBase* InActor);
	
	void SubscribeToGameState(TSubclassOf<APEPlayerCharacter> PossessedCharacter);
};