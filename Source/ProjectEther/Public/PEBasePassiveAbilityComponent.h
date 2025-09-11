// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Components/ActorComponent.h"
#include "PEBasePassiveAbilityComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class PROJECTETHER_API UPEBasePassiveAbilityComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPEBasePassiveAbilityComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Specs")
	TSubclassOf<UGameplayAbility> PassiveAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gameplay Effect")
	TSubclassOf<UGameplayEffect> GameplayEffect;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};