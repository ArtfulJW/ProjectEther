// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEBaseGameplayAbility.h"
#include "PEWarriorChallenge.generated.h"

class APEWarriorChallengeFlag;
/**
 * 
 */
UCLASS()
class PROJECTETHER_API UPEWarriorChallenge : public UPEBaseGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPEWarriorChallenge(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability Specs")
	TSubclassOf<APEWarriorChallengeFlag> BP_WarriorChallengeActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Spec")
	float fRange = 1000.0f;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION(Server, Reliable)
	void ServerSpawnWarriorChallenge(FHitResult Hit, APEPlayerCharacter* SourcePlayer, APEPlayerCharacter* TargetPlayer);
};