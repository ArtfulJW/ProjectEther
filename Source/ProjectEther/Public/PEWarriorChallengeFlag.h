// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "PEWarriorChallengeFlag.generated.h"

class APEPlayerCharacter;
class UGameplayEffect;
class USphereComponent;

UCLASS()
class PROJECTETHER_API APEWarriorChallengeFlag : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APEWarriorChallengeFlag();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability Specs")
	TSubclassOf<UGameplayEffect> ShamedGameplayEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability Specs")
	TSubclassOf<UGameplayEffect> VulnerableGameplayEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability Specs")
	USceneComponent* SceneComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability Specs")
	USphereComponent* SphereColliderComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability Specs")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY()
	TArray<APEPlayerCharacter*> PlayersWithinRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability Specs")
	float fChallengeRange =  500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability Specs")
	float fLifetime = 15.0f; 
	
	FGameplayEffectContextHandle EffectContext;

	FGameplayEffectSpecHandle ShamedEffectHandle;

	FGameplayEffectSpecHandle VulnerableEffectHandle;

	FActiveGameplayEffectHandle ActiveShamedEffectHandle;

	FActiveGameplayEffectHandle ActiveVulnerableEffectHandle;

	void ConstructGameplayEffectSpec(APEPlayerCharacter* TargetPlayer);

	void CheckPlayerDistance();

	void ApplyDebuffs(APEPlayerCharacter* PC);

	UFUNCTION()
	void DestroyAfterTime();

protected:
	virtual void BeginPlay() override;
};