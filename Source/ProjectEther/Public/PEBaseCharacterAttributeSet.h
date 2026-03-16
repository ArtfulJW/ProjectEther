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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_MaxHealth, Category = "Character Specs")
	FGameplayAttributeData MaxHealth;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Health, Category = "Character Specs")
	FGameplayAttributeData Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Speed, Category = "Character Specs")
	FGameplayAttributeData Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_AttackSpeed, Category="Character Specs")
	FGameplayAttributeData AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_AbilityCostMultiplier, Category="Character Specs")
	FGameplayAttributeData AbilityCostMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_CooldownMultiplier, Category="Character Specs")
	FGameplayAttributeData CooldownMultiplier;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_DamageDirectionFront, Category="Character Specs")
	FGameplayAttributeData DamageDirectionFront;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_DamageDirectionSide, Category="Character Specs")
	FGameplayAttributeData DamageDirectionSide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_DamageDirectionBack, Category="Character Specs")
	FGameplayAttributeData DamageDirectionBack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_JumpMagnitude, Category="Character Specs")
	FGameplayAttributeData JumpMagnitude;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Resource, Category="Character Specs")
	FGameplayAttributeData Resource;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_ResourceMaxValue, Category="Character Specs")
	FGameplayAttributeData ResourceMaxValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_ResourceReplenishRate, Category="Character Specs")
	FGameplayAttributeData ResourceReplenishRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Specs")
	FGameplayAttributeData OnTakeDamageResourceReplenishRate;
	
public:

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	void OnRep_Speed(const FGameplayAttributeData& OldSpeed);

	UFUNCTION()
	void OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed);

	UFUNCTION()
	void OnRep_AbilityCostMultiplier(const FGameplayAttributeData& OldAbilityCostMultiplier);

	UFUNCTION()
	void OnRep_CooldownMultiplier(const FGameplayAttributeData& OldCooldownMultiplier);
	
	UFUNCTION()
	void OnRep_DamageDirectionFront(const FGameplayAttributeData& OldDamageDirectionFront);

	UFUNCTION()
	void OnRep_DamageDirectionSide(const FGameplayAttributeData& OldDamageDirectionSide);

	UFUNCTION()
	void OnRep_DamageDirectionBack(const FGameplayAttributeData& OldDamageDirectionBack);

	UFUNCTION()
	void OnRep_JumpMagnitude(const FGameplayAttributeData& OldJumpMagnitude);

	UFUNCTION()
	void OnRep_Resource(const FGameplayAttributeData& OldResource);

	UFUNCTION()
	void OnRep_ResourceReplenishRate(const FGameplayAttributeData& OldResourceReplenishRate);

	UFUNCTION()
	void OnRep_ResourceMaxValue(const FGameplayAttributeData& OldResourceMaxValue);

	UFUNCTION(Server, Unreliable)
	virtual void AddResource(float InAmount);
	
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UPEBaseCharacterAttributeSet, MaxHealth);
	GAMEPLAYATTRIBUTE_VALUE_GETTER(MaxHealth);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(MaxHealth);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(MaxHealth);
	
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UPEBaseCharacterAttributeSet, Health);
	GAMEPLAYATTRIBUTE_VALUE_GETTER(Health);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(Health);

	UFUNCTION(Server, Reliable)
	void SetHealth(float InHealth);

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UPEBaseCharacterAttributeSet, Speed);
	GAMEPLAYATTRIBUTE_VALUE_GETTER(Speed);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(Speed);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(Speed);

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UPEBaseCharacterAttributeSet, AttackSpeed);
	GAMEPLAYATTRIBUTE_VALUE_GETTER(AttackSpeed);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(AttackSpeed);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(AttackSpeed);

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UPEBaseCharacterAttributeSet, AbilityCostMultiplier);
	GAMEPLAYATTRIBUTE_VALUE_GETTER(AbilityCostMultiplier);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(AbilityCostMultiplier);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(AbilityCostMultiplier);

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UPEBaseCharacterAttributeSet, CooldownMultiplier);
	GAMEPLAYATTRIBUTE_VALUE_GETTER(CooldownMultiplier);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(CooldownMultiplier);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(CooldownMultiplier);
	
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UPEBaseCharacterAttributeSet, DamageDirectionFront);
	GAMEPLAYATTRIBUTE_VALUE_GETTER(DamageDirectionFront);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(DamageDirectionFront);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(DamageDirectionFront);

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UPEBaseCharacterAttributeSet, DamageDirectionSide);
	GAMEPLAYATTRIBUTE_VALUE_GETTER(DamageDirectionSide);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(DamageDirectionSide);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(DamageDirectionSide);

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UPEBaseCharacterAttributeSet, DamageDirectionBack);
	GAMEPLAYATTRIBUTE_VALUE_GETTER(DamageDirectionBack);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(DamageDirectionBack);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(DamageDirectionBack);

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UPEBaseCharacterAttributeSet, JumpMagnitude);
	GAMEPLAYATTRIBUTE_VALUE_GETTER(JumpMagnitude);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(JumpMagnitude);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(JumpMagnitude);

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UPEBaseCharacterAttributeSet, Resource);
	GAMEPLAYATTRIBUTE_VALUE_GETTER(Resource);
	// GAMEPLAYATTRIBUTE_VALUE_SETTER(Resource);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(Resource);

	UFUNCTION(Server, Reliable)
	void SetResource(float InAmount);

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UPEBaseCharacterAttributeSet, ResourceMaxValue);
	GAMEPLAYATTRIBUTE_VALUE_GETTER(ResourceMaxValue);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(ResourceMaxValue);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(ResourceMaxValue);

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UPEBaseCharacterAttributeSet, ResourceReplenishRate);
	GAMEPLAYATTRIBUTE_VALUE_GETTER(ResourceReplenishRate);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(ResourceReplenishRate);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(ResourceReplenishRate);

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UPEBaseCharacterAttributeSet, OnTakeDamageResourceReplenishRate);
	GAMEPLAYATTRIBUTE_VALUE_GETTER(OnTakeDamageResourceReplenishRate);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(OnTakeDamageResourceReplenishRate);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(OnTakeDamageResourceReplenishRate);
};