// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEDeployRations.h"
#include "PEInteractableBase.h"
#include "PERations.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTETHER_API APERations : public APEInteractableBase
{
	GENERATED_BODY()
public:
	APERations();
	
	virtual void Interact(APEPlayerCharacter& InteractingPlayerCharacter) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Spec")
	TSubclassOf<UPEDeployRations> DeployRationsGameplayAbility;
	
	FActiveGameplayEffectHandle ActiveRefreshedEffectHandle;

	FGameplayEffectSpecHandle RefreshedEffectSpecHandle;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastDestroyRation();
};