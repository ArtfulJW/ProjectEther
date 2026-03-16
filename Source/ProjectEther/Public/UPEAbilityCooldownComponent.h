// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UPEAbilityCooldownComponent.generated.h"

class APEPlayerCharacter;
/**
 * 
 */
UCLASS(Blueprintable)
class PROJECTETHER_API UPEAbilityCooldownComponent : public UObject
{
	GENERATED_BODY()
public:
	UPEAbilityCooldownComponent();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_ReadyChanged, VisibleAnywhere, BlueprintReadOnly, Category = "Ability Spec")
	bool bIsReadyToExecute = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Spec")
	float fMinCooldownTime = 0.1f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Spec")
	float fMaxCooldownTime = 120.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Spec")
	float fCooldownTime = 1.0f;

	FTimerHandle TimerHandle;
	
	/*
	 * Checks if this ability is ready for execution
	 */
	bool CheckIsAbilityReady();

	UFUNCTION(Server, Reliable)
	void ServerSetIsReadyToExecute(bool inBool);

	UFUNCTION()
	void RefreshAbilityCooldown();

	UFUNCTION()
	void OnRep_ReadyChanged(bool inBool);
};
