// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEBasePassiveAbilityActor.h"
#include "PEPlayerCharacter.h"
#include "PEFrostTouchedPassiveComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTETHER_API APEFrostTouchedPassiveComponent : public APEBasePassiveAbilityActor
{
	GENERATED_BODY()
public:
	APEFrostTouchedPassiveComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Specs")
	TSubclassOf<UGameplayEffect> GE_Frost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Specs")
	bool bBindToPlayerOnHitDelegate;

	FGameplayEffectContextHandle FrostTouchedContextHandle;

	FGameplayEffectSpecHandle FrostTouchedEffectSpecHandle;
	
	FActiveGameplayEffectHandle ActiveFrostTouchedEffectHandle;

	FGameplayEffectSpec FrostTouchedEffectSpec;
	
	void OnHit(APEPlayerCharacter* HitPlayer);

	/*
	 * Generic Function to construct Gameplay effect for use
	 */
	FGameplayEffectSpec ConstructGameplayEffect(FGameplayEffectContextHandle InContextHandle,
		FGameplayEffectSpecHandle InSpecHandle,
		TSubclassOf<UGameplayEffect> InGameplayEffect) const;

	UFUNCTION(Server, Reliable)
	void ServerApplyOnHitGameplayEffect(const APEPlayerCharacter* OverlappedPlayerCharacter);
	
protected:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPassiveAbility() override;
};
