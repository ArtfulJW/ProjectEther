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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Specs")
	FGameplayAttributeData Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Specs")
	FGameplayAttributeData Speed;

public:
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UPEBaseCharacterAttributeSet, Health);
	GAMEPLAYATTRIBUTE_VALUE_GETTER(Health);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(Health);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(Health);

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UPEBaseCharacterAttributeSet, Speed);
	GAMEPLAYATTRIBUTE_VALUE_GETTER(Speed);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(Speed);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(Speed);
};