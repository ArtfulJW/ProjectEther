// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEBasePassiveAbilityActor.h"
#include "PEPlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "PECrushingPresencePassive.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTETHER_API APECrushingPresencePassive : public APEBasePassiveAbilityActor
{
	GENERATED_BODY()

public:
	APECrushingPresencePassive();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Specs")
	USphereComponent* SphereColliderComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Specs")
	TSubclassOf<UGameplayAbility> PassiveAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Specs")
	TSubclassOf<UGameplayEffect> GameplayEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Specs")
	int32 NumHits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Specs")
	int32 NumHitsToReset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Specs")
	float fOverlapDelayToApplyBrokenWill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Specs")
	bool bBindToPlayerOnHitDelegate;
	
	FGameplayEffectContextHandle EffectContext;

	FGameplayEffectSpecHandle BrokenWillEffectHandle;
	
	FActiveGameplayEffectHandle ActiveBrokenWillEffectHandle;

	FGameplayEffectSpec* BrokenWillEffectSpec;

	UPROPERTY()
	TArray<APEPlayerCharacter*> OverlappedPlayerCharacters;
	
	virtual void SetupPassiveAbility() override;
	
	void OnHit(APEPlayerCharacter* HitPlayer);

	UFUNCTION()
	void OverlapEvent(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex);
	
	UFUNCTION(Server, Reliable)
	void ServerApplyOnHitGameplayEffect(const APEPlayerCharacter* OverlappedPlayerCharacter);

	UFUNCTION()
	void ApplyGameplayEffectAfterTimer(const APEPlayerCharacter* OverlappedPlayerCharacter);

	void ConstructGameplayEffectSpec();
	
protected:
	virtual void BeginPlay() override;
};