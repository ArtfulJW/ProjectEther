// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEBasePassiveAbilityActor.h"
#include "PEPlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "PEBirthrightApostlePassive.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTETHER_API APEBirthrightApostlePassive : public APEBasePassiveAbilityActor
{
	GENERATED_BODY()
public:
	APEBirthrightApostlePassive();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Specs")
	USphereComponent* SphereColliderComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Specs")
	TSubclassOf<UGameplayEffect> GE_HolyPresence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Specs")
	TSubclassOf<UGameplayEffect> GE_GuardianAngel;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Specs")
	bool bBindToPlayerLookingAtTeammateDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Specs")
	float fPassiveLookAtDistance;

	FGameplayEffectContextHandle HolyPresenceEffectContext;

	FGameplayEffectSpecHandle HolyPresenceEffectSpecHandle;
	
	FActiveGameplayEffectHandle ActiveHolyPresenceEffectHandle;

	FGameplayEffectSpec HolyPresenceEffectSpec;

	FGameplayEffectContextHandle GuardianAngelEffectContext;
	
	FGameplayEffectSpecHandle GuardianAngelEffectSpecHandle;
	
	FActiveGameplayEffectHandle ActiveGuardianAngelEffectHandle;
	
	FGameplayEffectSpec GuardianAngelEffectSpec;

	UPROPERTY()
	APEPlayerCharacter* LookedAtTeammate;

	bool bAlreadyAppliedGuardianAngelEffect;
	
	/*
	 * SphereCollider functions to process who is within range 'us' and apply healing gameplay effect (Holy Presence)
	 */
	UFUNCTION()
	void OverlapEvent(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/*
	 * Player no longer in range of us, remove healing gameplay effect (Holy Presence)
	 */
	UFUNCTION()
	void EndOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex);

	/*
	 * Generic Function to construct Gameplay effect for use
	 */
	FGameplayEffectSpec ConstructGameplayEffect(FGameplayEffectContextHandle InContextHandle, FGameplayEffectSpecHandle InSpecHandle,
	                                            TSubclassOf<UGameplayEffect> InGameplayEffect) const;

	APEPlayerCharacter* LookingAtTeammate();
	
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
};
