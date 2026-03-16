// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayAbilitySpecHandle.h"
#include "InputMappingContext.h"
#include "PEInteractableBase.h"
#include "PEEtherWarStructs.h"
#include "PEPickClassHUD.h"
#include "PEPlayerController.generated.h"

class UPEAbilityCooldownComponent;
class UPEGameOptionsOverlay;
class APEEquipmentCache;
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
	APEPlayerController();
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* CheckCompassAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* RunAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* OptionsAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spectator")
	TSubclassOf<ASpectatorPawn> PESpectatorPawn;

	UPROPERTY(Replicated, VisibleAnywhere, Category="Class Spec")
	EClassType CharacterClass = EClassType::Base;

	/**
	 * TODO: REMOVE LATER AFTER PROTOTYPE
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD")
	TSubclassOf<UPEPickClassHUD> PEPickClassHUDClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="HUD")
	UUserWidget* PEPickClassHUD;
	
	/**
	 * TODO: TEMPORARILY HERE TO SPAWN PLAYER CHARACTER. REMOVE LATER
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player Character")
	TSubclassOf<APEPlayerCharacter> PEPlayerCharacterClass;
	
	/**
	 * TODO: TEMPORARILY HERE TO SPAWN PLAYER CHARACTER. REMOVE LATER
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player Character")
	TSubclassOf<APEPlayerCharacter> PEBerserkerPlayerCharacterClass;

	/**
	 * TODO: TEMPORARILY HERE TO SPAWN PLAYER CHARACTER. REMOVE LATER
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player Character")
	TSubclassOf<APEPlayerCharacter> PEMagePlayerCharacterClass;

	/**
	 * TODO: TEMPORARILY HERE TO SPAWN PLAYER CHARACTER. REMOVE LATER
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player Character")
	TSubclassOf<APEPlayerCharacter> PEPriestPlayerCharacterClass;
	
	UPROPERTY(Replicated, VisibleAnywhere)
	ETeam Team;

	UPROPERTY(Replicated)
	bool bCanBasicAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD")
	TSubclassOf<UPEGameOptionsOverlay> OptionsOverlayClass;

	UPROPERTY()
	UPEGameOptionsOverlay* GameOptionsOverlay = nullptr;

	bool TempBIsReadyToExecute = true;
	
	void MoveEvent(const FInputActionValue& Value);

	UFUNCTION(Server, Unreliable)
	void ServerMovePlayer(APEPlayerController* Requester, FVector InVector);
	
	void LookEvent(const FInputActionValue& Value);

	UFUNCTION(Server, Unreliable)
	void ServerLookEvent(APEPlayerController* Requester, FRotator InRotator);
	
	void UseAbilityEvent(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void ServerUseGameplayAbilityEvent(APEPlayerController* Requester, FGameplayAbilitySpecHandle AbilityHandle, UAnimMontage* InAnimMontage, float
	                                   ResourceCost, float CurrentResourceAmnt);

	void UseWeaponEvent(const FInputActionValue& Value);

	bool IsPossessingSpectatorPawn(APEPlayerController* Requester);
	
	void InteractEvent();

	void DeployInteractableEvent();

	void CheckCompassEvent();

	void JumpEvent();

	void RunEvent();

	UFUNCTION(Server, Reliable)
	void ServerInteractEvent(APEPlayerController* Requester, AActor* InActor);

	UFUNCTION(Server, Reliable)
	void ServerDropInteractableActor(APEPlayerController* Requester);

	UFUNCTION(Server, Reliable)
	void ServerDeployInteractable(APEInteractableBase* InActor);
	
	void SubscribeToGameState(TSubclassOf<APEPlayerCharacter> PossessedCharacter);

	UFUNCTION(Server, Reliable)
	void ServerDestroyEquipmentCache(APEEquipmentCache* EquipmentCache);

	UFUNCTION(Server, Reliable)
	void ServerCheckCompassEvent(APEPlayerController* Requester);

	UFUNCTION(Client, Reliable)
	void ClientSetupInputControls();
	
	UFUNCTION(Server, Reliable)
	void RequestSpawn();

	UFUNCTION(Server, Unreliable)
	void ServerSetCharacterClass(APEPlayerController* Requester, EClassType InClassType);

	UFUNCTION(Client, Reliable)
	void ClientRemovePickClassHUD();

	UFUNCTION(Server, Reliable)
	void RequestTeamAssignment();

	void ReplenishBasicAttack();

	UFUNCTION(Server, Reliable)
	void ServerJumpEvent();

	UFUNCTION(Server, Reliable)
	void ServerRunEvent();

	UFUNCTION(Server, Reliable)
	void ServerRequestSpectatorSpawn();

	UFUNCTION(Client, Reliable)
	void ClientProcessActivateHitIndicator(EDirectionDamageIndicator InDirectionDamageIndicator, FLinearColor InColor);

	UFUNCTION()
	void OptionsWindowEvent();

	UFUNCTION(Server, Reliable)
	void ServerCheckToPlayFPSAnimations(APEPlayerCharacter* PC, UAnimMontage* InAnimMontage, FGameplayAbilitySpecHandle AbilityHandle);

	UFUNCTION(Client, Reliable)
	void ClientPlayFPSAnim(APEPlayerCharacter* PC, UAnimMontage* InAnimMontage);
	
protected:
	virtual void OnPossess(APawn* APawn) override;
};