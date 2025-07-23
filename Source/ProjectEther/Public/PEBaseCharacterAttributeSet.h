// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "PEBaseCharacterAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTETHER_API UPEBaseCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Health, Category = "Character Specs")
	FGameplayAttributeData Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Speed, Category = "Character Specs")
	FGameplayAttributeData Speed;

public:

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	void OnRep_Speed(const FGameplayAttributeData& OldSpeed);
	
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UPEBaseCharacterAttributeSet, Health);
	GAMEPLAYATTRIBUTE_VALUE_GETTER(Health);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(Health);

	UFUNCTION(Server, Reliable)
	void SetHealth(float InHealth);

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UPEBaseCharacterAttributeSet, Speed);
	GAMEPLAYATTRIBUTE_VALUE_GETTER(Speed);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(Speed);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(Speed);
};